#include "PapyrusInterface.h"

#include "LibidoManager.h"

float PapyrusInterface::GetTotalLibido(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return LibidoManager::GetSingleton()->GetTotalLibido(actorRef);
}

bool PapyrusInterface::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("GetTotalLibido", "OSLArousedNative", GetTotalLibido);
	return true;
}
