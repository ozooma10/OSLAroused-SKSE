#include "Papyrus.h"
#include "ArousalManager.h"
#include "Serialization.h"
#include "RuntimeEvents.h"
#include "Settings.h"
#include "Debug.h"

#define BIND(a_method, ...) a_vm->RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)

void Papyrus::UpdatePlayerNudityCheck(RE::StaticFunctionTag*, bool newVal)
{
	Settings::GetSingleton()->SetPlayerNudityCheckEnabled(newVal);
	if (newVal) {
		//starts Poller if stopped
		WorldChecks::NakedArousalTicker::GetSingleton()->Start();
		logger::trace("Starting PlayerNakedUpdateLoop...");
	} else {
		//Stops Poller if running
		WorldChecks::NakedArousalTicker::GetSingleton()->Stop();
		logger::trace("Stopping PlayerNakedUpdateLoop...");
	}
}

void Papyrus::UpdateArousalMode(RE::StaticFunctionTag*, int newArousalMode)
{
	logger::trace("UpdateArousalMode: {}", newArousalMode);

	Settings::ArousalMode newMode;
	switch (newArousalMode) {
	case 0:
		newMode = Settings::ArousalMode::kSexlabAroused;
		break;
	case 1:
		newMode = Settings::ArousalMode::kOAroused;
		break;
	default:
		return;
	}

	Settings::GetSingleton()->SetArousalMode(newMode);
	logger::trace("Arousal Mode Updated...");
}

void Papyrus::UpdateDefaultArousalMultiplier(RE::StaticFunctionTag*, float newMultiplier)
{
	Settings::GetSingleton()->SetDefaultArousalMultiplier(newMultiplier);
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

float Papyrus::GetArousalMultiplier(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return Serialization::MultiplierData::GetSingleton()->GetData(actorRef->formID, Settings::GetSingleton()->GetDefaultArousalMultiplier());
}

float Papyrus::GetExposure(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	//If we are in sla mode get exposure, otherwise just return arousal
	if (Settings::GetSingleton()->GetArousalMode() == Settings::ArousalMode::kSexlabAroused) {
		float curTime = RE::Calendar::GetSingleton()->GetCurrentGameTime();
		auto lastCheckTime = Serialization::LastCheckTimeData::GetSingleton()->GetData(actorRef->formID, 0.f);
		Serialization::LastCheckTimeData::GetSingleton()->SetData(actorRef->formID, curTime);
		return ArousalManager::GetSexlabExposure(actorRef, curTime - lastCheckTime);
	}

	return ArousalManager::GetArousal(actorRef);
}

float Papyrus::GetDaysSinceLastOrgasm(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	float lastOrgasmTime = Serialization::LastOrgasmTimeData::GetSingleton()->GetData(actorRef->formID);
	if (lastOrgasmTime < 0) {
		lastOrgasmTime = 0;
	}

	return RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastOrgasmTime;
}

void Papyrus::SetTimeRate(RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	if (!actorRef) {
		return;
	}
	value = std::clamp(value, 0.f, 100.f);
	Serialization::TimeRateData::GetSingleton()->SetData(actorRef->formID, value);
}

float Papyrus::GetTimeRate(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return Serialization::TimeRateData::GetSingleton()->GetData(actorRef->formID, 10.0);
}

void Papyrus::SetPlayerInSexScene(RE::StaticFunctionTag*, bool bInScene)
{
	Settings::GetSingleton()->SetPlayerInSexScene(bInScene);
}

void Papyrus::DumpArousalData(RE::StaticFunctionTag*)
{
	Debug::DumpAllArousalData();
}

void Papyrus::ClearSecondaryArousalData(RE::StaticFunctionTag*)
{
	Debug::ClearSecondaryArousalData();
}

void Papyrus::ClearAllArousalData(RE::StaticFunctionTag*)
{
	Debug::ClearAllArousalData();
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("UpdatePlayerNudityCheck", "OSLArousedNative", UpdatePlayerNudityCheck);
	vm->RegisterFunction("UpdateArousalMode", "OSLArousedNative", UpdateArousalMode);
	vm->RegisterFunction("UpdateDefaultArousalMultiplier", "OSLArousedNative", UpdateDefaultArousalMultiplier);

	vm->RegisterFunction("GetArousal", "OSLArousedNative", GetArousal);
	vm->RegisterFunction("GetArousalMultiple", "OSLArousedNative", GetArousalMultiple);
	vm->RegisterFunction("SetArousal", "OSLArousedNative", SetArousal);
	vm->RegisterFunction("SetArousalMultiple", "OSLArousedNative", SetArousalMultiple);
	vm->RegisterFunction("ModifyArousal", "OSLArousedNative", ModifyArousal);
	vm->RegisterFunction("ModifyArousalMultiple", "OSLArousedNative", ModifyArousalMultiple);

	vm->RegisterFunction("SetArousalMultiplier", "OSLArousedNative", SetArousalMultiplier);
	vm->RegisterFunction("GetArousalMultiplier", "OSLArousedNative", GetArousalMultiplier);

	vm->RegisterFunction("GetExposure", "OSLArousedNative", GetExposure);
	vm->RegisterFunction("GetDaysSinceLastOrgasm", "OSLArousedNative", GetDaysSinceLastOrgasm);

	vm->RegisterFunction("SetTimeRate", "OSLArousedNative", SetTimeRate);
	vm->RegisterFunction("GetTimeRate", "OSLArousedNative", GetTimeRate);

	vm->RegisterFunction("SetPlayerInSexScene", "OSLArousedNative", SetPlayerInSexScene);

	vm->RegisterFunction("DumpArousalData", "OSLArousedNative", DumpArousalData);
	vm->RegisterFunction("ClearSecondaryArousalData", "OSLArousedNative", ClearSecondaryArousalData);
	vm->RegisterFunction("ClearAllArousalData", "OSLArousedNative", ClearAllArousalData);


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
