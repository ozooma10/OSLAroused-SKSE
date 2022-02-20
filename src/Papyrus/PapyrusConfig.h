#pragma once

namespace PapyrusConfig
{
	//OSLAroused Settings
	void SetArousalChangeRate(RE::StaticFunctionTag* base, float newVal);
	void SetLibidoChangeRate(RE::StaticFunctionTag* base, float newVal);

	void SetSceneParticipantBaseline(RE::StaticFunctionTag* base, float newVal);
	void SetSceneViewingBaseline(RE::StaticFunctionTag* base, float newVal);
	void SetSceneVictimGainsArousal(RE::StaticFunctionTag* base, bool newVal);
	void SetBeingNudeBaseline(RE::StaticFunctionTag* base, float newVal);
	void SetViewingNudeBaseline(RE::StaticFunctionTag* base, float newVal);

	void SetEroticArmorBaseline(RE::StaticFunctionTag* base, float newVal, RE::BGSKeyword* keyword);

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* vm);
}
