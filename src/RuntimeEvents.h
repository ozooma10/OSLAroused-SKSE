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
	void ArousalUpdateLoop();

	class AurousalUpdateTicker : public Utilities::Ticker
	{
	public:
		AurousalUpdateTicker(std::chrono::milliseconds interval) :
			Utilities::Ticker(std::function<void()>(ArousalUpdateLoop), interval) {}

		static AurousalUpdateTicker* GetSingleton()
		{
			static AurousalUpdateTicker singleton(std::chrono::milliseconds(15000));
			return &singleton;
		}

		float LastUpdatePollGameTime = RE::Calendar::GetSingleton()->GetHoursPassed();
	};
}
