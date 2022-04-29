#pragma once

namespace PapyrusActor
{
	std::vector<RE::TESForm*> GetAllEquippedArmor(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
};
