from __future__ import annotations

from abc import ABC, abstractmethod

from app.schemas import AgentRequest, AgentResponseData


class AgentProvider(ABC):
    name: str
    model: str

    @abstractmethod
    def generate(self, request: AgentRequest) -> AgentResponseData:
        """Return a structured response without executing flight actions."""
