#include "PapyrusConfig.h"
#include "Settings.h"

void PapyrusConfig::SetArousalChangeRate(RE::StaticFunctionTag*, float newVal)
{
	Settings::GetSingleton()->SetArousalChangeRate(newVal);
}

void PapyrusConfig::SetLibidoChangeRate(RE::StaticFunctionTag*, float newVal)
{
	Settings::GetSingleton()->SetLibidoChangeRate(newVal);
}

void PapyrusConfig::SetSceneParticipantBaseline(RE::StaticFunctionTag*, float newVal)
{
	Settings::GetSingleton()->SetSceneParticipantBaseline(newVal);
}

void PapyrusConfig::SetSceneViewingBaseline(RE::StaticFunctionTag*, float newVal)
{
	Settings::GetSingleton()->SetSceneViewingBaseline(newVal);
}

void PapyrusConfig::SetSceneVictimGainsArousal(RE::StaticFunctionTag*, bool newVal)
{
	Settings::GetSingleton()->SetSceneVictimGainsArousal(newVal);
}

void PapyrusConfig::SetBeingNudeBaseline(RE::StaticFunctionTag*, float newVal)
{
	Settings::GetSingleton()->SetNudeArousalBaseline(newVal);
}

void PapyrusConfig::SetViewingNudeBaseline(RE::StaticFunctionTag*, float newVal)
{
	Settings::GetSingleton()->SetNudeViewingBaseline(newVal);
}

bool PapyrusConfig::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("SetArousalChangeRate", "OSLArousedNativeConfig", SetArousalChangeRate);
	vm->RegisterFunction("SetLibidoChangeRate", "OSLArousedNativeConfig", SetLibidoChangeRate);

	vm->RegisterFunction("SetSceneParticipantBaseline", "OSLArousedNativeConfig", SetSceneParticipantBaseline);
	vm->RegisterFunction("SetSceneViewingBaseline", "OSLArousedNativeConfig", SetSceneViewingBaseline);
	vm->RegisterFunction("SetSceneVictimGainsArousal", "OSLArousedNativeConfig", SetSceneVictimGainsArousal);
	vm->RegisterFunction("SetBeingNudeBaseline", "OSLArousedNativeConfig", SetBeingNudeBaseline);
	vm->RegisterFunction("SetViewingNudeBaseline", "OSLArousedNativeConfig", SetViewingNudeBaseline);
	
	return true;
}
