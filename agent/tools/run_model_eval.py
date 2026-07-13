from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

AGENT_ROOT = Path(__file__).resolve().parents[1]
if str(AGENT_ROOT) not in sys.path:
    sys.path.insert(0, str(AGENT_ROOT))

from app.config import AgentSettings
from app.providers.ollama import OllamaProvider
from app.schemas import AgentContext, AgentRequest


DEFAULT_CASES = AGENT_ROOT / "tests" / "fixtures" / "model_eval_cases.json"
EVAL_ALLOWED_CAPABILITIES = [
    "vehicle.query_status",
    "vehicle.query_battery",
    "vehicle.query_position",
    "vehicle.query_rtk",
    "log.explain_error",
    "ui.select_vehicle",
    "ui.open_page",
    "map.focus_coordinate",
    "mission.create_draft",
    "mission.analyze",
    "vehicle.arm",
    "vehicle.takeoff",
    "vehicle.land",
    "vehicle.rtl",
    "vehicle.pause",
    "vehicle.goto",
    "mission.upload",
    "mission.start",
    "param.write",
    "mavlink.send_raw",
]


def main() -> int:
    parser = argparse.ArgumentParser(description="Run optional local Ollama model stability evaluation.")
    parser.add_argument("--run-real-model", action="store_true", help="Required to call local Ollama.")
    parser.add_argument("--cases", default=str(DEFAULT_CASES), help="Path to model_eval_cases.json.")
    parser.add_argument("--limit", type=int, default=0, help="Optional maximum number of cases.")
    args = parser.parse_args()

    if not args.run_real_model:
        print("Refusing to call a real model without --run-real-model.")
        return 2

    cases = json.loads(Path(args.cases).read_text(encoding="utf-8"))
    if args.limit > 0:
        cases = cases[: args.limit]

    provider = OllamaProvider(AgentSettings(provider="ollama"))
    health = provider.health()
    if not health.ready:
        print(f"provider_ready=false error={health.error}")
        return 3

    total = len(cases)
    command_matches = 0
    proposal_matches = 0
    failures: list[str] = []

    for index, case in enumerate(cases, start=1):
        request = AgentRequest(
            request_id=f"eval-{index}",
            session_id="model-eval",
            message=case["input"],
            context=AgentContext(),
            allowed_capabilities=EVAL_ALLOWED_CAPABILITIES,
        )
        try:
            response = provider.generate(request)
        except Exception as exc:  # noqa: BLE001 - evaluation summary only.
            failures.append(f"{index}: provider_error={type(exc).__name__}")
            continue

        actual_command = response.proposal.command if response.proposal else None
        actual_has_proposal = response.proposal is not None
        if actual_command == case["expected_command"]:
            command_matches += 1
        else:
            failures.append(f"{index}: command expected={case['expected_command']} actual={actual_command}")
        if actual_has_proposal == case["should_have_proposal"]:
            proposal_matches += 1

    print(f"total={total}")
    print(f"command_match={command_matches}/{total}")
    print(f"proposal_shape_match={proposal_matches}/{total}")
    print(f"failures={len(failures)}")
    for item in failures[:20]:
        print(item)
    if len(failures) > 20:
        print(f"... {len(failures) - 20} more failures omitted")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
