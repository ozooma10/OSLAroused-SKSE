#pragma once

namespace Papyrus
{
	void UpdatePlayerNudityCheck(RE::StaticFunctionTag* base, bool newVal);

	void UpdateArousalMode(RE::StaticFunctionTag* base, int newArousalMode);

	void UpdateDefaultArousalMultiplier(RE::StaticFunctionTag* base, float newMultiplier);

	float GetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	std::vector<float> GetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs);

	void SetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void SetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	void ModifyArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void ModifyArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	void SetArousalMultiplier(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);

	float GetArousalMultiplier(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	float GetExposure(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float GetDaysSinceLastOrgasm(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float GetTimeRate(RE::StaticFunctionTag* base, RE::Actor* actorRef);



	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);

	namespace Events
	{
		void SendPlayerArousalUpdatedEvent(float newVal);
	}

};
