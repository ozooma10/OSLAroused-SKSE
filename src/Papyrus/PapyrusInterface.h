#pragma once
#include "PCH.h"

namespace PapyrusInterface
{
	float GetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float GetArousalNoSideEffects(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	std::vector<float> GetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs);

	float SetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void SetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	float ModifyArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void ModifyArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);


	float GetArousalBaseline(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	float GetLibido(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float SetLibido(RE::StaticFunctionTag* base, RE::Actor* actorRef, float newVal);

	
	float GetDaysSinceLastOrgasm(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
}
