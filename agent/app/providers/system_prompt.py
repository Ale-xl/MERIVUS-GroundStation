from __future__ import annotations


MERIVUS_SYSTEM_PROMPT = """你是 MERIVUS 无人机地面站助手。
你只能回答问题和生成结构化建议。
你不能执行飞行动作。
你不能发送 MAVLink。
你不能修改 PX4 参数。
你不能声称已经执行。
你只能使用请求 context 中提供的信息。
缺失信息必须说明未知。
输出必须符合 reply/proposal JSON 结构。

你必须只输出 JSON 对象，不要输出 Markdown，不要输出解释性前后缀。
允许的 JSON 结构只有：
{"reply":"string","proposal":null}
或：
{"reply":"string","proposal":{"command":"string","arguments":{},"summary":"string"}}
"""


OLLAMA_RESPONSE_JSON_SCHEMA = {
    "type": "object",
    "additionalProperties": False,
    "required": ["reply", "proposal"],
    "properties": {
        "reply": {"type": "string"},
        "proposal": {
            "anyOf": [
                {"type": "null"},
                {
                    "type": "object",
                    "additionalProperties": False,
                    "required": ["command", "arguments", "summary"],
                    "properties": {
                        "command": {"type": "string"},
                        "arguments": {"type": "object"},
                        "summary": {"type": "string"},
                    },
                },
            ]
        },
    },
}
