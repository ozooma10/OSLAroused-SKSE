#pragma once
#include <Utilities.h>

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

namespace WorldChecks
{
	void PlayerNakedUpdateLoop();

	class NakedArousalTicker : public Utilities::Ticker
	{
	public:
		NakedArousalTicker(std::chrono::milliseconds interval) :
			Utilities::Ticker(std::function<void()>(PlayerNakedUpdateLoop), interval) {}

		static NakedArousalTicker* GetSingleton()
		{
			static NakedArousalTicker singleton(std::chrono::milliseconds(2000));
			return &singleton;
		}

		float LastNakedPollGameTime = RE::Calendar::GetSingleton()->GetHoursPassed();
	};
}
