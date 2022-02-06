#pragma once

namespace Papyrus
{
	//OSLAroused Settings
	void UpdatePlayerNudityCheck(RE::StaticFunctionTag* base, bool newVal);
	void UpdateHourlyNudityArousalModifier(RE::StaticFunctionTag* base, float newVal);
	void UpdateArousalMode(RE::StaticFunctionTag* base, int newArousalMode);
	void UpdateDefaultArousalMultiplier(RE::StaticFunctionTag* base, float newMultiplier);

	//General State
	float GetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	std::vector<float> GetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs);
	float GetArousalNoSideEffects(RE::StaticFunctionTag* base, RE::Actor* actorRef);

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

	//Actor State
	bool IsActorNaked(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	void RegisterSceneStart(RE::StaticFunctionTag* base, bool bIsOstim, int sceneId, RE::reference_array<RE::Actor*> actorRefs);
	void RemoveScene(RE::StaticFunctionTag* base, bool bIsOstim, int sceneId);

	void RegisterActorOrgasm(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	//Keyword Management
	bool AddKeywordToForm(RE::StaticFunctionTag* base, RE::TESForm* form, RE::BGSKeyword* keyword);
	bool RemoveKeywordFromForm(RE::StaticFunctionTag* base, RE::TESForm* form, RE::BGSKeyword* keyword);

	//Utilities
	float GenerateRandomFloat(RE::StaticFunctionTag* base, float min, float max);

	//Debug Functions
	void DumpArousalData(RE::StaticFunctionTag* base);
	void ClearSecondaryArousalData(RE::StaticFunctionTag* base);
	void ClearAllArousalData(RE::StaticFunctionTag* base);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);

	namespace Events
	{
		void SendActorArousalUpdatedEvent(RE::Actor* actorRef, float newVal);

		void SendActorNakedUpdatedEvent(RE::Actor* actorRef, bool newNaked);
	}

};
