#pragma once
#include "PCH.h"

namespace PapyrusInterface
{
	float GetArousalBaseline(RE::StaticFunctionTag* base, RE::Actor* actorRef);

	float GetLibido(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	float SetLibido(RE::StaticFunctionTag* base, RE::Actor* actorRef, float newVal);


	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
}
