#pragma once

namespace Papyrus
{
	void UpdatePlayerNudityCheck(RE::StaticFunctionTag* base, bool newVal);

	void UpdateHourlyNudityArousalModifier(RE::StaticFunctionTag* base, float newVal);

	void UpdateArousalMode(RE::StaticFunctionTag* base, int newArousalMode);

	void UpdateDefaultArousalMultiplier(RE::StaticFunctionTag* base, float newMultiplier);

	float GetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	std::vector<float> GetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs);

	float SetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void SetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	float ModifyArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void ModifyArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	void SetArousalMultiplier(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	float GetArousalMultiplier(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	float GetExposure(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float GetDaysSinceLastOrgasm(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	void SetTimeRate(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	float GetTimeRate(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	void SetPlayerInSexScene(RE::StaticFunctionTag* base, bool bInScene);

	//Keyword Management
	bool AddKeywordToForm(RE::StaticFunctionTag* base, RE::TESForm* form, RE::BGSKeyword* keyword);
	bool RemoveKeywordFromForm(RE::StaticFunctionTag* base, RE::TESForm* form, RE::BGSKeyword* keyword);

	//Debug Functions
	void DumpArousalData(RE::StaticFunctionTag* base);
	void ClearSecondaryArousalData(RE::StaticFunctionTag* base);
	void ClearAllArousalData(RE::StaticFunctionTag* base);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);

	namespace Events
	{
		void SendPlayerArousalUpdatedEvent(float newVal);
	}

};
