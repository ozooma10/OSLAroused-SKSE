#include "Papyrus.h"
#include "PersistedData.h"
#include "RuntimeEvents.h"
#include "Settings.h"
#include "Debug.h"
#include "Utilities/Utils.h"
#include "Managers/ActorStateManager.h"
#include "Managers/ArousalManager.h"
#include "Managers/SceneManager.h"

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

float Papyrus::GetArousalNoSideEffects(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return ArousalManager::GetArousal(actorRef, false);
}

float Papyrus::SetArousal(RE::StaticFunctionTag*, RE::Actor*, float)
{
	return 0.f;
	//return ArousalManager::SetArousal(actorRef, value);
}

void Papyrus::SetArousalMultiple(RE::StaticFunctionTag*, RE::reference_array<RE::Actor*>, float)
{
	//for (const auto actorRef : actorRefs) {
		//ArousalManager::SetArousal(actorRef, value);
	//}
}

float Papyrus::ModifyArousal(RE::StaticFunctionTag*, RE::Actor* actorRef, float value)
{
	return ArousalManager::ModifyArousal(actorRef, value);
}

void Papyrus::ModifyArousalMultiple(RE::StaticFunctionTag*, RE::reference_array<RE::Actor*> actorRefs, float value)
{
	for (const auto actorRef : actorRefs) {
		ArousalManager::ModifyArousal(actorRef, value);
	}
}

float Papyrus::GetDaysSinceLastOrgasm(RE::StaticFunctionTag*, RE::Actor* )
{
	//float lastOrgasmTime = Serialization::LastOrgasmTimeData::GetSingleton()->GetData(actorRef->formID, 0.f);
	//if (lastOrgasmTime < 0) {
		//lastOrgasmTime = 0;
	//}

	//return RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastOrgasmTime;
	return 0.f;
}

bool Papyrus::IsActorNaked(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return ActorStateManager::GetSingleton()->GetActorNaked(actorRef);
}

void Papyrus::RegisterSceneStart(RE::StaticFunctionTag*, bool bIsOstim, int sceneId, RE::reference_array<RE::Actor*> actorRefs)
{
	SceneManager::SceneData sceneData {
		bIsOstim ? SceneManager::SceneFramework::kOStim : SceneManager::SceneFramework::kSexLab,
		sceneId,
		actorRefs
	};
	SceneManager::GetSingleton()->RegisterScene(sceneData);
}

void Papyrus::RemoveScene(RE::StaticFunctionTag*, bool bIsOstim, int sceneId)
{
	SceneManager::GetSingleton()->RemoveScene(bIsOstim ? SceneManager::SceneFramework::kOStim : SceneManager::SceneFramework::kSexLab, sceneId);
}

void Papyrus::RegisterActorOrgasm(RE::StaticFunctionTag*, RE::Actor* )//actorRef)
{
	//Serialization::LastOrgasmTimeData::GetSingleton()->SetData(actorRef->formID, RE::Calendar::GetSingleton()->GetCurrentGameTime());
}

bool Papyrus::AddKeywordToForm(RE::StaticFunctionTag*, RE::TESForm* form, RE::BGSKeyword* keyword)
{
	if (!form || !keyword) {
		return false;
	}
	
	return Utilities::Keywords::AddKeyword(form, keyword);
}

bool Papyrus::RemoveKeywordFromForm(RE::StaticFunctionTag*, RE::TESForm* form, RE::BGSKeyword* keyword)
{
	if (!form || !keyword) {
		return false;
	}

	return Utilities::Keywords::RemoveKeyword(form, keyword);
}

float Papyrus::GenerateRandomFloat(RE::StaticFunctionTag*, float min, float max)
{
	return Utilities::GenerateRandomFloat(min, max);
}

float Papyrus::ClampFloat(RE::StaticFunctionTag*, float val, float min, float max)
{
	return val > max ? max : (val < min ? min : val);
}

void Papyrus::DumpArousalData(RE::StaticFunctionTag*)
{
	Debug::DumpAllArousalData();
}

void Papyrus::ClearAllArousalData(RE::StaticFunctionTag*)
{
	Debug::ClearAllArousalData();
}

bool Papyrus::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	//General State
	vm->RegisterFunction("GetArousal", "OSLArousedNative", GetArousal);
	vm->RegisterFunction("GetArousalMultiple", "OSLArousedNative", GetArousalMultiple);
	vm->RegisterFunction("GetArousalNoSideEffects", "OSLArousedNative", GetArousalNoSideEffects);

	vm->RegisterFunction("SetArousal", "OSLArousedNative", SetArousal);
	vm->RegisterFunction("SetArousalMultiple", "OSLArousedNative", SetArousalMultiple);
	vm->RegisterFunction("ModifyArousal", "OSLArousedNative", ModifyArousal);
	vm->RegisterFunction("ModifyArousalMultiple", "OSLArousedNative", ModifyArousalMultiple);

	vm->RegisterFunction("GetDaysSinceLastOrgasm", "OSLArousedNative", GetDaysSinceLastOrgasm);
	
	//Actor State
	vm->RegisterFunction("IsActorNaked", "OSLArousedNative", IsActorNaked);

	vm->RegisterFunction("RegisterSceneStart", "OSLArousedNative", RegisterSceneStart);
	vm->RegisterFunction("RemoveScene", "OSLArousedNative", RemoveScene);

	vm->RegisterFunction("RegisterActorOrgasm", "OSLArousedNative", RegisterActorOrgasm);

	//Keyword
	vm->RegisterFunction("AddKeywordToForm", "OSLArousedNative", AddKeywordToForm);
	vm->RegisterFunction("RemoveKeywordFromForm", "OSLArousedNative", RemoveKeywordFromForm);

	//Utilities
	vm->RegisterFunction("GenerateRandomFloat", "OSLArousedNative", GenerateRandomFloat);
	
	//Debug
	vm->RegisterFunction("DumpArousalData", "OSLArousedNative", DumpArousalData);
	vm->RegisterFunction("ClearAllArousalData", "OSLArousedNative", ClearAllArousalData);

	return true;
}

void SendModEvent(RE::BSFixedString eventName, float numArg, RE::TESForm* sender)
{
	SKSE::ModCallbackEvent modEvent{
		eventName,
		RE::BSFixedString(),
		numArg,
		sender
	};

	auto modCallback = SKSE::GetModCallbackEventSource();
	modCallback->SendEvent(&modEvent);
}

void Papyrus::Events::SendActorArousalUpdatedEvent(RE::Actor* actorRef, float newExposure)
{
	SendModEvent("OSLA_ActorArousalUpdated", newExposure, actorRef);
}

void Papyrus::Events::SendActorLibidoUpdatedEvent(RE::Actor* actorRef, float newLibido)
{
	SendModEvent("OSLA_ActorLibidoUpdated", newLibido, actorRef);
}

void Papyrus::Events::SendActorNakedUpdatedEvent(RE::Actor* actorRef, bool newNaked)
{
	SendModEvent("OSLA_ActorNakedUpdated", newNaked ? 1.f : 0.f, actorRef);
}
