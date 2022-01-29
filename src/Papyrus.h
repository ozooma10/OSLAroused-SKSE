#pragma once

namespace Papyrus
{
	float GetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef);
	std::vector<float> GetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs);

	void SetArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void SetArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	void ModifyArousal(RE::StaticFunctionTag* base, RE::Actor* actorRef, float value);
	void ModifyArousalMultiple(RE::StaticFunctionTag* base, RE::reference_array<RE::Actor*> actorRefs, float value);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);

	namespace Events
	{
		void SendPlayerArousalUpdatedEvent(float newVal);
	}

};
