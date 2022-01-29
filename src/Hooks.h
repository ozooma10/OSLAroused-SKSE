#pragma once
#include "PCH.h"

namespace Hooks
{
	class ActorEquipHook
	{
	public:
		static void InstallHook()
		{
			REL::Relocation<std::uintptr_t> actorItemEquipped{ RE::Offset::ActorEquipManager::EquipObject };
			logger::info("InstallHook EQUIP OBJECT HOOK");

			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();
			m_EquipObject = trampoline.write_call<5>(actorItemEquipped.address(), EquipObject);
		}

	private:
		static void EquipObject(RE::Actor* a_actor, RE::TESBoundObject* a_object, RE::ExtraDataList* a_extraData, std::uint32_t a_count, const RE::BGSEquipSlot* a_slot, bool a_queueEquip, bool a_forceEquip, bool a_playSounds, bool a_applyNow);
		static inline REL::Relocation<decltype(EquipObject)> m_EquipObject;
	};
}
