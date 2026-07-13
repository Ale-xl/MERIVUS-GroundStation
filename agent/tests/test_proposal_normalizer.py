import json
from pathlib import Path

import pytest

from app.providers.base import ProviderError
from app.providers.ollama import OllamaProvider
from app.proposal_normalizer import classify_user_intent, normalize_model_response, normalize_proposal


FIXTURE_PATH = Path(__file__).parent / "fixtures" / "model_eval_cases.json"


def test_command_alias_takeoff_is_normalized():
    result = normalize_proposal(
        {"command": "takeoff", "arguments": {"drone_id": "1", "target_altitude": 10}, "summary": "take off"},
        "reply",
    )

    assert result.proposal is not None
    assert result.proposal.command == "vehicle.takeoff"
    assert result.proposal.arguments == {"vehicle_id": 1, "altitude_m": 10.0}


def test_command_alias_rtl_and_pause_are_normalized():
    rtl = normalize_proposal({"command": "RTL", "arguments": {"uav_id": 3}, "summary": "rtl"}, "reply")
    pause = normalize_proposal({"command": "hold", "arguments": {"vehicle": 2}, "summary": "hold"}, "reply")

    assert rtl.proposal.command == "vehicle.rtl"
    assert rtl.proposal.arguments == {"vehicle_id": 3}
    assert pause.proposal.command == "vehicle.pause"
    assert pause.proposal.arguments == {"vehicle_id": 2}


def test_parameter_aliases_are_normalized_for_goto():
    result = normalize_proposal(
        {"command": "goto", "arguments": {"drone": "2", "lat": "31.2", "lng": "121.4", "alt": "60"}, "summary": "goto"},
        "reply",
    )

    assert result.proposal is not None
    assert result.proposal.command == "vehicle.goto"
    assert result.proposal.arguments == {
        "vehicle_id": 2,
        "latitude": 31.2,
        "longitude": 121.4,
        "altitude_m": 60.0,
    }


def test_dangerous_fields_are_stripped_without_granting_authority():
    response = normalize_model_response(
        {
            "reply": "suggestion only",
            "risk": "Low",
            "executed": True,
            "proposal": {
                "command": "vehicle.takeoff",
                "arguments": {"vehicle_id": 1, "altitude": 10, "shell": "do it"},
                "summary": "takeoff",
                "executable": True,
            },
        }
    )

    assert response.proposal is not None
    assert response.proposal.command == "vehicle.takeoff"
    assert response.proposal.arguments == {"vehicle_id": 1, "altitude_m": 10.0}
    assert "executed" not in response.model_dump()


def test_unknown_command_becomes_null_proposal():
    response = normalize_model_response(
        {"reply": "not sure", "proposal": {"command": "fly_magic", "arguments": {}, "summary": "magic"}}
    )

    assert response.proposal is None
    assert "无法形成结构化建议" in response.reply


def test_invalid_arguments_become_null_proposal():
    response = normalize_model_response(
        {"reply": "bad goto", "proposal": {"command": "vehicle.goto", "arguments": {"lat": 91, "lon": 121}, "summary": "bad"}}
    )

    assert response.proposal is None
    assert "参数缺失或越界" in response.reply


def test_missing_vehicle_id_is_null_for_flight_target_commands():
    response = normalize_model_response(
        {"reply": "land suggestion", "proposal": {"command": "land", "arguments": {}, "summary": "land"}}
    )

    assert response.proposal.command == "vehicle.land"
    assert response.proposal.arguments == {"vehicle_id": None}


def test_missing_takeoff_altitude_is_not_defaulted():
    response = normalize_model_response(
        {"reply": "takeoff suggestion", "proposal": {"command": "takeoff", "arguments": {"uav_id": 1}, "summary": "takeoff"}}
    )

    assert response.proposal.command == "vehicle.takeoff"
    assert response.proposal.arguments == {"vehicle_id": 1}


def test_qa_fault_question_overrides_model_position_proposal():
    response = normalize_model_response(
        {
            "reply": "未获得有效位置估计通常与GPS、EKF或传感器数据质量有关。",
            "proposal": {"command": "vehicle.query_position", "arguments": {"vehicle_id": None}, "summary": "查询位置"},
        },
        user_message="未获得有效位置估计和EKF2报警是什么原因？",
    )

    assert response.proposal is None
    assert "当前请求没有提供真实遥测" in response.reply


def test_explicit_position_query_keeps_query_proposal():
    response = normalize_model_response(
        {
            "reply": "准备查询一号机位置。",
            "proposal": {"command": "vehicle.query_position", "arguments": {"vehicle_id": 1}, "summary": "查询一号机位置"},
        },
        user_message="查询一号机位置",
    )

    assert response.proposal is not None
    assert response.proposal.command == "vehicle.query_position"
    assert response.proposal.arguments == {"vehicle_id": 1}


def test_intent_classifier_separates_qa_query_and_flight_request():
    assert classify_user_intent("EKF2报警是什么意思？") == "log_explanation"
    assert classify_user_intent("查询一号机位置") == "status_query"
    assert classify_user_intent("让一号机起飞到10米") == "flight_proposal"


def test_model_eval_fixture_has_required_shape_and_size():
    cases = json.loads(FIXTURE_PATH.read_text(encoding="utf-8"))

    assert len(cases) >= 50
    for case in cases:
        assert isinstance(case["input"], str) and case["input"].strip()
        assert "expected_command" in case
        assert case["expected_risk_class"] in {"Informational", "Low", "Medium", "High", "Critical"}
        assert isinstance(case["should_have_proposal"], bool)
        assert isinstance(case["notes"], str)


def test_ollama_provider_uses_normalizer_for_aliases():
    response = OllamaProvider._validated_response(
        {
            "reply": "suggestion only",
            "proposal": {"command": "takeoff", "arguments": {"target_vehicle": 1, "height": 10}, "summary": "take off"},
        }
    )

    assert response.proposal.command == "vehicle.takeoff"
    assert response.proposal.arguments == {"vehicle_id": 1, "altitude_m": 10.0}


def test_ollama_provider_still_rejects_missing_reply():
    with pytest.raises(ProviderError) as exc_info:
        OllamaProvider._validated_response({"proposal": None})

    assert exc_info.value.code == "model_output_invalid_schema"
