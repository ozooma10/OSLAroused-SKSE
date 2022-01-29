#include "Hooks.h"

void Hooks::ActorEquipHook::EquipObject(RE::Actor* a_actor, RE::TESBoundObject* a_object, RE::ExtraDataList* a_extraData, std::uint32_t a_count, const RE::BGSEquipSlot* a_slot, bool a_queueEquip, bool a_forceEquip, bool a_playSounds, bool a_applyNow)
{
	logger::info("EQUIP OBJECT HOOK");

	m_EquipObject(a_actor, a_object, a_extraData, a_count, a_slot, a_queueEquip, a_forceEquip, a_playSounds, a_applyNow);
}
