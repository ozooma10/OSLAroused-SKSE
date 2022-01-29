#pragma once

namespace ArousalManager
{
	float GetArousal(RE::Actor* actorRef);
	float SetArousal(RE::Actor* actorRef, float value);
	void ModifyArousal(RE::Actor* actorRef, float value);
}
