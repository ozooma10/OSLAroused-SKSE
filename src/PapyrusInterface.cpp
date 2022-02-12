#include "PapyrusInterface.h"

#include "Managers/LibidoManager.h"

float PapyrusInterface::GetArousalBaseline(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return LibidoManager::GetSingleton()->GetBaselineArousal(actorRef);
}

float PapyrusInterface::GetLibido(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return LibidoManager::GetSingleton()->GetBaseLibido(actorRef);
}

float PapyrusInterface::SetLibido(RE::StaticFunctionTag*, RE::Actor* actorRef, float newVal)
{
	return LibidoManager::GetSingleton()->SetBaseLibido(actorRef, newVal);
}

bool PapyrusInterface::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("GetArousalBaseline", "OSLArousedNative", GetArousalBaseline);
	vm->RegisterFunction("GetLibido", "OSLArousedNative", GetLibido);
	vm->RegisterFunction("SetLibido", "OSLArousedNative", SetLibido);
	return true;
}
