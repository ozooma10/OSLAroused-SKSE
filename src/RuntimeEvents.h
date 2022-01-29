#pragma once

namespace RuntimeEvents
{
	class OnEquipEvent : public RE::BSTEventSink<RE::TESEquipEvent>
	{
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESEquipEvent* equipEvent, RE::BSTEventSource<RE::TESEquipEvent>*) override;

	public:
		static bool RegisterEvent() {
			static OnEquipEvent g_EquipEventHandler;
			auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();
			if (!ScriptEventSource) {
				return false;
			}
			ScriptEventSource->AddEventSink(&g_EquipEventHandler);
			return true;
		}
	};
}
