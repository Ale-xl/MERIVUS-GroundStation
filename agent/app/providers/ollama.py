from __future__ import annotations

import json
from typing import Any

import httpx
from pydantic import ValidationError

from app.config import AgentSettings
from app.providers.base import AgentProvider, ProviderError, ProviderHealth, ProviderInfo
from app.providers.system_prompt import MERIVUS_SYSTEM_PROMPT, OLLAMA_RESPONSE_JSON_SCHEMA
from app.schemas import AgentRequest, AgentResponseData, Proposal


FORBIDDEN_MODEL_FIELDS = {
    "executed",
    "executable",
    "risk",
    "localRisk",
    "local_risk",
    "policyDecision",
    "policy_decision",
    "requiresConfirmation",
    "requires_confirmation",
    "mavlink",
    "shell",
    "script",
    "px4_parameters",
}


class OllamaProvider(AgentProvider):
    name = "ollama"

    def __init__(self, settings: AgentSettings, client: httpx.Client | None = None) -> None:
        self.base_url = settings.ollama_base_url.rstrip("/")
        self.model = settings.ollama_model
        self.timeout_seconds = max(1, settings.ollama_timeout_seconds)
        self._client = client or httpx.Client(base_url=self.base_url, timeout=self.timeout_seconds)

    def health(self) -> ProviderHealth:
        try:
            models = self._available_models()
        except ProviderError as exc:
            return ProviderHealth(ready=False, error=exc.message)

        if self.model not in models:
            return ProviderHealth(
                ready=False,
                error=f"Model {self.model} is not installed",
                available_models=models,
            )

        return ProviderHealth(ready=True, available_models=models)

    def info(self) -> ProviderInfo:
        health = self.health()
        return ProviderInfo(
            provider=self.name,
            model=self.model,
            provider_ready=health.ready,
            provider_error=health.error,
            available_models=health.available_models,
            external_network_enabled=False,
            flight_execution_enabled=False,
        )

    def generate(self, request: AgentRequest) -> AgentResponseData:
        health = self.health()
        if not health.ready:
            raise ProviderError("provider_not_ready", health.error or "Ollama provider is not ready")

        payload = {
            "model": self.model,
            "stream": False,
            "think": False,
            "format": OLLAMA_RESPONSE_JSON_SCHEMA,
            "messages": [
                {"role": "system", "content": MERIVUS_SYSTEM_PROMPT},
                {"role": "user", "content": self._request_content(request)},
            ],
        }

        response = self._post_chat(payload)
        content = self._extract_message_content(response)
        data = self._parse_model_json(content)
        return self._validated_response(data)

    def _available_models(self) -> list[str]:
        try:
            response = self._client.get("/api/tags")
            response.raise_for_status()
        except httpx.TimeoutException as exc:
            raise ProviderError("ollama_timeout", "Ollama service request timed out") from exc
        except httpx.ConnectError as exc:
            raise ProviderError("ollama_unavailable", "Ollama service is not available") from exc
        except httpx.HTTPStatusError as exc:
            raise ProviderError("ollama_http_error", f"Ollama returned HTTP {exc.response.status_code}") from exc
        except httpx.RequestError as exc:
            raise ProviderError("ollama_unavailable", "Ollama service is not available") from exc

        body = self._json_response(response)
        models = body.get("models")
        if not isinstance(models, list):
            raise ProviderError("ollama_invalid_response", "Ollama tags response is invalid")

        names: list[str] = []
        for item in models:
            if isinstance(item, dict) and isinstance(item.get("name"), str):
                names.append(item["name"])
        return names

    def _post_chat(self, payload: dict[str, Any]) -> dict[str, Any]:
        try:
            response = self._client.post("/api/chat", json=payload)
            response.raise_for_status()
        except httpx.TimeoutException as exc:
            raise ProviderError("ollama_timeout", "Ollama chat request timed out") from exc
        except httpx.ConnectError as exc:
            raise ProviderError("ollama_unavailable", "Ollama service is not available") from exc
        except httpx.HTTPStatusError as exc:
            raise ProviderError("ollama_http_error", f"Ollama returned HTTP {exc.response.status_code}") from exc
        except httpx.RequestError as exc:
            raise ProviderError("ollama_unavailable", "Ollama service is not available") from exc
        return self._json_response(response)

    @staticmethod
    def _json_response(response: httpx.Response) -> dict[str, Any]:
        try:
            body = response.json()
        except ValueError as exc:
            raise ProviderError("ollama_invalid_json", "Ollama response is not valid JSON") from exc
        if not isinstance(body, dict):
            raise ProviderError("ollama_invalid_json", "Ollama response is not a JSON object")
        return body

    @staticmethod
    def _extract_message_content(response: dict[str, Any]) -> str:
        message = response.get("message")
        if not isinstance(message, dict) or not isinstance(message.get("content"), str):
            raise ProviderError("ollama_invalid_response", "Ollama chat response is missing message.content")
        return message["content"]

    @staticmethod
    def _parse_model_json(content: str) -> dict[str, Any]:
        try:
            data = json.loads(content)
        except json.JSONDecodeError as exc:
            raise ProviderError("model_output_invalid_json", "Model output is not valid JSON") from exc
        if not isinstance(data, dict):
            raise ProviderError("model_output_invalid_schema", "Model output must be a JSON object")
        return data

    @staticmethod
    def _validated_response(data: dict[str, Any]) -> AgentResponseData:
        forbidden = _find_forbidden_field(data)
        if forbidden:
            raise ProviderError("model_output_forbidden_field", f"Model output contains forbidden field: {forbidden}")

        allowed_keys = {"reply", "proposal"}
        extra_keys = set(data) - allowed_keys
        if extra_keys:
            raise ProviderError("model_output_invalid_schema", f"Model output contains unsupported field: {sorted(extra_keys)[0]}")

        reply = data.get("reply")
        if not isinstance(reply, str) or not reply.strip():
            raise ProviderError("model_output_invalid_schema", "Model output must include a non-empty reply string")

        raw_proposal = data.get("proposal")
        if raw_proposal is None:
            return AgentResponseData(reply=reply, proposal=None)

        if not isinstance(raw_proposal, dict):
            raise ProviderError("model_output_invalid_schema", "proposal must be an object or null")

        proposal_keys = {"command", "arguments", "summary"}
        extra_proposal_keys = set(raw_proposal) - proposal_keys
        if extra_proposal_keys:
            raise ProviderError(
                "model_output_invalid_schema",
                f"proposal contains unsupported field: {sorted(extra_proposal_keys)[0]}",
            )

        if not isinstance(raw_proposal.get("arguments"), dict):
            raise ProviderError("model_output_invalid_schema", "proposal.arguments must be an object")

        try:
            proposal = Proposal.model_validate(raw_proposal)
        except ValidationError as exc:
            raise ProviderError("model_output_invalid_schema", "proposal does not match the required schema") from exc
        return AgentResponseData(reply=reply, proposal=proposal)

    @staticmethod
    def _request_content(request: AgentRequest) -> str:
        return json.dumps(
            {
                "message": request.message,
                "context": request.context.model_dump(),
                "allowed_capabilities": request.allowed_capabilities,
            },
            ensure_ascii=False,
        )


def _find_forbidden_field(value: Any) -> str | None:
    if isinstance(value, dict):
        for key, child in value.items():
            if key in FORBIDDEN_MODEL_FIELDS:
                return key
            nested = _find_forbidden_field(child)
            if nested:
                return nested
    elif isinstance(value, list):
        for child in value:
            nested = _find_forbidden_field(child)
            if nested:
                return nested
    return None
