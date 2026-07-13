from __future__ import annotations

from dataclasses import dataclass
from typing import Any

from app.schemas import AgentResponseData, Proposal


STANDARD_COMMANDS = {
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
}

COMMAND_ALIASES = {
    "status": "vehicle.query_status",
    "query_status": "vehicle.query_status",
    "vehicle.status": "vehicle.query_status",
    "查看状态": "vehicle.query_status",
    "查询状态": "vehicle.query_status",
    "无人机状态": "vehicle.query_status",
    "电量": "vehicle.query_battery",
    "battery": "vehicle.query_battery",
    "query_battery": "vehicle.query_battery",
    "gps": "vehicle.query_position",
    "position": "vehicle.query_position",
    "定位": "vehicle.query_position",
    "rtk": "vehicle.query_rtk",
    "log": "log.explain_error",
    "error_log": "log.explain_error",
    "解释日志": "log.explain_error",
    "select_vehicle": "ui.select_vehicle",
    "选择无人机": "ui.select_vehicle",
    "open_page": "ui.open_page",
    "打开页面": "ui.open_page",
    "focus_coordinate": "map.focus_coordinate",
    "地图定位": "map.focus_coordinate",
    "mission_draft": "mission.create_draft",
    "任务草稿": "mission.create_draft",
    "mission_analysis": "mission.analyze",
    "mission.analyse": "mission.analyze",
    "分析航线": "mission.analyze",
    "arm": "vehicle.arm",
    "解锁": "vehicle.arm",
    "force_arm": "vehicle.arm",
    "强制解锁": "vehicle.arm",
    "takeoff": "vehicle.takeoff",
    "take off": "vehicle.takeoff",
    "起飞": "vehicle.takeoff",
    "起飞到10米": "vehicle.takeoff",
    "land": "vehicle.land",
    "降落": "vehicle.land",
    "rtl": "vehicle.rtl",
    "return_to_launch": "vehicle.rtl",
    "return to launch": "vehicle.rtl",
    "返航": "vehicle.rtl",
    "回家": "vehicle.rtl",
    "pause": "vehicle.pause",
    "hold": "vehicle.pause",
    "暂停": "vehicle.pause",
    "悬停": "vehicle.pause",
    "goto": "vehicle.goto",
    "go_to": "vehicle.goto",
    "飞到坐标": "vehicle.goto",
    "upload_mission": "mission.upload",
    "上传任务": "mission.upload",
    "start_mission": "mission.start",
    "启动任务": "mission.start",
    "write_param": "param.write",
    "参数写入": "param.write",
    "send_mavlink": "mavlink.send_raw",
    "mavlink": "mavlink.send_raw",
}

ARGUMENT_ALIASES = {
    "drone_id": "vehicle_id",
    "drone": "vehicle_id",
    "uav_id": "vehicle_id",
    "uav": "vehicle_id",
    "vehicle": "vehicle_id",
    "target_vehicle": "vehicle_id",
    "aircraft_id": "vehicle_id",
    "vehicleId": "vehicle_id",
    "height": "altitude_m",
    "altitude": "altitude_m",
    "alt": "altitude_m",
    "takeoff_height": "altitude_m",
    "target_altitude": "altitude_m",
    "targetAltitude": "altitude_m",
    "lat": "latitude",
    "latitude": "latitude",
    "lng": "longitude",
    "lon": "longitude",
    "longitude": "longitude",
    "page": "page",
    "view": "page",
    "panel": "page",
}

ALLOWED_ARGUMENTS = {
    "vehicle.query_status": {"vehicle_id"},
    "vehicle.query_battery": {"vehicle_id"},
    "vehicle.query_position": {"vehicle_id"},
    "vehicle.query_rtk": {"vehicle_id"},
    "log.explain_error": {"vehicle_id"},
    "ui.select_vehicle": {"vehicle_id"},
    "ui.open_page": {"page"},
    "map.focus_coordinate": {"latitude", "longitude"},
    "mission.create_draft": {"vehicle_id"},
    "mission.analyze": {"vehicle_id"},
    "vehicle.arm": {"vehicle_id"},
    "vehicle.takeoff": {"vehicle_id", "altitude_m"},
    "vehicle.land": {"vehicle_id"},
    "vehicle.rtl": {"vehicle_id"},
    "vehicle.pause": {"vehicle_id"},
    "vehicle.goto": {"vehicle_id", "latitude", "longitude", "altitude_m"},
    "mission.upload": {"vehicle_id"},
    "mission.start": {"vehicle_id"},
    "param.write": {"name", "value"},
    "mavlink.send_raw": set(),
}

DANGEROUS_FIELDS = {
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

FLIGHT_TARGET_COMMANDS = {
    "vehicle.arm",
    "vehicle.takeoff",
    "vehicle.land",
    "vehicle.rtl",
    "vehicle.pause",
    "vehicle.goto",
    "mission.upload",
    "mission.start",
}


@dataclass(frozen=True)
class NormalizedProposalResult:
    reply: str
    proposal: Proposal | None
    normalized: bool
    reason: str | None = None


def normalize_model_response(data: dict[str, Any]) -> AgentResponseData:
    reply = data.get("reply")
    if not isinstance(reply, str) or not reply.strip():
        raise ValueError("Model output must include a non-empty reply string")

    raw_proposal = data.get("proposal")
    result = normalize_proposal(raw_proposal, reply.strip())
    return AgentResponseData(reply=result.reply, proposal=result.proposal)


def normalize_proposal(raw_proposal: Any, reply: str) -> NormalizedProposalResult:
    if raw_proposal is None:
        return NormalizedProposalResult(reply=reply, proposal=None, normalized=True)

    if not isinstance(raw_proposal, dict):
        return _no_structured_proposal(reply, "proposal 不是对象或 null")

    cleaned = _strip_dangerous_fields(raw_proposal)
    command = _normalize_command(cleaned.get("command"))
    if command is None:
        return _no_structured_proposal(reply, "无法将模型命令映射到标准 command")

    arguments_raw = cleaned.get("arguments")
    if arguments_raw is None:
        arguments_raw = {}
    if not isinstance(arguments_raw, dict):
        return _no_structured_proposal(reply, "proposal.arguments 不是对象")

    arguments = _normalize_arguments(command, arguments_raw)
    if arguments is None:
        return _no_structured_proposal(reply, "参数缺失或越界，无法形成结构化建议")

    summary = cleaned.get("summary")
    if not isinstance(summary, str) or not summary.strip():
        return _no_structured_proposal(reply, "proposal.summary 缺失")

    try:
        proposal = Proposal(command=command, arguments=arguments, summary=summary.strip())
    except Exception:
        return _no_structured_proposal(reply, "规范化后的 proposal 未通过 Agent schema")

    return NormalizedProposalResult(reply=reply, proposal=proposal, normalized=True)


def _normalize_command(command: Any) -> str | None:
    if not isinstance(command, str):
        return None
    value = command.strip()
    if not value:
        return None
    if value in STANDARD_COMMANDS:
        return value
    key = value.lower().replace("_", ".").strip()
    if key in STANDARD_COMMANDS:
        return key
    return COMMAND_ALIASES.get(value) or COMMAND_ALIASES.get(value.lower())


def _normalize_arguments(command: str, raw_arguments: dict[str, Any]) -> dict[str, Any] | None:
    allowed = ALLOWED_ARGUMENTS.get(command)
    if allowed is None:
        return None

    normalized: dict[str, Any] = {}
    for key, value in raw_arguments.items():
        if key in DANGEROUS_FIELDS:
            continue
        canonical_key = ARGUMENT_ALIASES.get(key, key)
        if canonical_key not in allowed:
            continue
        coerced = _coerce_argument(canonical_key, value)
        if coerced is _INVALID:
            return None
        normalized[canonical_key] = coerced

    if command in FLIGHT_TARGET_COMMANDS and "vehicle_id" not in normalized:
        normalized["vehicle_id"] = None

    if command == "ui.select_vehicle" and not isinstance(normalized.get("vehicle_id"), int):
        return None

    if command == "ui.open_page" and not normalized.get("page"):
        return None

    if command in {"map.focus_coordinate", "vehicle.goto"}:
        if not _valid_coordinate(normalized.get("latitude"), normalized.get("longitude")):
            return None
        if command == "vehicle.goto" and not _valid_altitude(normalized.get("altitude_m"), allow_zero=True):
            return None

    if command == "vehicle.takeoff" and "altitude_m" in normalized:
        if not _valid_altitude(normalized.get("altitude_m"), allow_zero=False):
            return None

    if command == "mavlink.send_raw":
        return {}

    return normalized


_INVALID = object()


def _coerce_argument(key: str, value: Any) -> Any:
    if key == "vehicle_id":
        if value is None:
            return None
        if isinstance(value, bool):
            return _INVALID
        if isinstance(value, int):
            return value if value > 0 else _INVALID
        if isinstance(value, str):
            stripped = value.strip()
            if stripped.isdigit():
                parsed = int(stripped)
                return parsed if parsed > 0 else _INVALID
        return _INVALID

    if key in {"altitude_m", "latitude", "longitude"}:
        if isinstance(value, bool):
            return _INVALID
        if isinstance(value, (int, float)):
            return float(value)
        if isinstance(value, str):
            try:
                return float(value.strip())
            except ValueError:
                return _INVALID
        return _INVALID

    if key in {"page", "name"}:
        if not isinstance(value, str):
            return _INVALID
        stripped = value.strip()
        return stripped if stripped else _INVALID

    return value


def _valid_coordinate(latitude: Any, longitude: Any) -> bool:
    return (
        isinstance(latitude, (int, float))
        and isinstance(longitude, (int, float))
        and -90.0 <= float(latitude) <= 90.0
        and -180.0 <= float(longitude) <= 180.0
    )


def _valid_altitude(value: Any, allow_zero: bool) -> bool:
    if not isinstance(value, (int, float)):
        return False
    minimum = 0.0 if allow_zero else 0.0
    if allow_zero:
        return minimum <= float(value) <= 120.0
    return minimum < float(value) <= 120.0


def _strip_dangerous_fields(value: Any) -> Any:
    if isinstance(value, dict):
        return {
            key: _strip_dangerous_fields(child)
            for key, child in value.items()
            if key not in DANGEROUS_FIELDS
        }
    if isinstance(value, list):
        return [_strip_dangerous_fields(child) for child in value]
    return value


def _no_structured_proposal(reply: str, reason: str) -> NormalizedProposalResult:
    suffix = f"\n\n无法形成结构化建议：{reason}。"
    return NormalizedProposalResult(reply=reply + suffix, proposal=None, normalized=False, reason=reason)
