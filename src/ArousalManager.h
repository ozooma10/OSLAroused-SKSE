#pragma once

namespace ArousalManager
{
	float GetArousal(RE::Actor* actorRef);
	float SetArousal(RE::Actor* actorRef, float value);
	void ModifyArousal(RE::Actor* actorRef, float value);

	float GetActorTimeRate(RE::Actor* actorRef, float timeSinceLastOrgasm);

	float GetSexlabExposure(RE::Actor* actorRef, float timePassed);
}
