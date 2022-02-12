#pragma once

namespace PapyrusConfig
{
	//OSLAroused Settings
	void SetSceneParticipantBaseline(RE::StaticFunctionTag* base, float newVal);
	void SetSceneViewingBaseline(RE::StaticFunctionTag* base, float newVal);
	void SetSceneVictimGainsArousal(RE::StaticFunctionTag* base, bool newVal);
	void SetBeingNudeBaseline(RE::StaticFunctionTag* base, float newVal);
	void SetViewingNudeBaseline(RE::StaticFunctionTag* base, float newVal);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
}
