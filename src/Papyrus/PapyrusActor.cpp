#include "D:/Skyrim/OSLAroused-SKSE/build/src/CMakeFiles/OSLAroused_SKSE.dir/Debug/cmake_pch.hxx"
#include "PapyrusActor.h"
#include "Utilities/Utils.h"

std::vector<RE::TESForm*> PapyrusActor::GetAllEquippedArmor(RE::StaticFunctionTag*, RE::Actor* actorRef)
{
	return Utilities::Actor::GetWornArmor(actorRef);
}

bool PapyrusActor::RegisterFunctions(RE::BSScript::IVirtualMachine* vm)
{
	vm->RegisterFunction("GetAllEquippedArmor", "OSLArousedNativeActor", GetAllEquippedArmor);
	return false;
}
