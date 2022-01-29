#include "Papyrus.h"
#include "ArousalManager.h"
#include "Serialization.h"
#include "RuntimeEvents.h"
#include "Settings.h"

#define BIND(a_method, ...) a_vm->RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)

void Papyrus::UpdatePlayerNudityCheck(RE::StaticFunctionTag*, bool newVal)
{
	Settings::GetSingleton()->SetPlayerNudityCheckEnabled(newVal);
	if (newVal) {
		//starts Poller if stopped
		WorldChecks::g_PlayerNakedUpdateLoopTicker.Start();
	} else {
		//Stops Poller if running
		WorldChecks::g_PlayerNakedUpdateLoopTicker.Stop();
	}
}

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

void Papyrus::SetArousalMultiplier(RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	//Just clamp between 0 and 100?
	value = std::clamp(value, 0.0f, 100.f);

	Serialization::MultiplierData::GetSingleton()->SetData(actorRef->formID, value);
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("UpdatePlayerNudityCheck", "OSLArousedNative", UpdatePlayerNudityCheck);

	vm->RegisterFunction("GetArousal", "OSLArousedNative", GetArousal);
	vm->RegisterFunction("GetArousalMultiple", "OSLArousedNative", GetArousalMultiple);
	vm->RegisterFunction("SetArousal", "OSLArousedNative", SetArousal);
	vm->RegisterFunction("SetArousalMultiple", "OSLArousedNative", SetArousalMultiple);
	vm->RegisterFunction("ModifyArousal", "OSLArousedNative", ModifyArousal);
	vm->RegisterFunction("ModifyArousalMultiple", "OSLArousedNative", ModifyArousalMultiple);

	vm->RegisterFunction("SetArousalMultiplier", "OSLArousedNative", SetArousalMultiplier);
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
