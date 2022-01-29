#include "Papyrus.h"
#include <ArousalManager.h>

float Papyrus::GetArousal(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return ArousalManager::GetArousal(actorRef);
}

std::vector<float> Papyrus::GetArousalMultiple(RE::StaticFunctionTag*, RE::reference_array<RE::Actor*> actorRefs)
{
	std::vector<float> results;

	for (const auto actorRef : actorRefs) {
		results.push_back(ArousalManager::GetArousal(actorRef));
	}

	return results;
}

void Papyrus::SetArousal(RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	ArousalManager::SetArousal(actorRef, value);
}

void Papyrus::SetArousalMultiple(RE::StaticFunctionTag*, RE::reference_array<RE::Actor*> actorRefs, float value)
{
	for (const auto actorRef : actorRefs) {
		ArousalManager::SetArousal(actorRef, value);
	}
}

void Papyrus::ModifyArousal(RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	ArousalManager::ModifyArousal(actorRef, value);
}

void Papyrus::ModifyArousalMultiple(RE::StaticFunctionTag*, RE::reference_array<RE::Actor*> actorRefs, float value)
{
	for (const auto actorRef : actorRefs) {
		ArousalManager::ModifyArousal(actorRef, value);
	}
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("GetArousal", "OSLArousalNative", GetArousal);
	vm->RegisterFunction("GetArousalMultiple", "OSLArousalNative", GetArousalMultiple);
	vm->RegisterFunction("SetArousal", "OSLArousalNative", SetArousal);
	vm->RegisterFunction("SetArousalMultiple", "OSLArousalNative", SetArousalMultiple);
	vm->RegisterFunction("ModifyArousal", "OSLArousalNative", ModifyArousal);
	vm->RegisterFunction("ModifyArousalMultiple", "OSLArousalNative", ModifyArousalMultiple);
	return true;
}

void Papyrus::Events::SendPlayerArousalUpdatedEvent(float newVal)
{
	SKSE::ModCallbackEvent playerArousalUpdatedEvent{
		"OSLA_PlayerArousalUpdated",
		RE::BSFixedString(),
		newVal,
		nullptr
	};

	auto modCallback = SKSE::GetModCallbackEventSource();
	modCallback->SendEvent(&playerArousalUpdatedEvent);
}
