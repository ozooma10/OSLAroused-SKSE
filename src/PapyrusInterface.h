#pragma once
#include "PCH.h"

namespace PapyrusInterface
{
	float GetTotalLibido(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
}
