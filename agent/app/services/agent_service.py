from __future__ import annotations

from app.config import DEFAULT_MODEL
from app.providers import AgentProvider, MockProvider
from app.schemas import AgentRequest, AgentResponseData


class UnknownProviderError(ValueError):
    pass


class AgentService:
    def __init__(self, provider_name: str) -> None:
        self.provider = self._create_provider(provider_name)

    @property
    def provider_name(self) -> str:
        return self.provider.name

    @property
    def model(self) -> str:
        return self.provider.model

    def generate(self, request: AgentRequest) -> AgentResponseData:
        return self.provider.generate(request)

    @staticmethod
    def _create_provider(provider_name: str) -> AgentProvider:
        if provider_name == "mock":
            return MockProvider()
        raise UnknownProviderError(f"Provider is not available: {provider_name}")


def provider_model(provider_name: str) -> str:
    if provider_name == "mock":
        return DEFAULT_MODEL
    return "unavailable"
