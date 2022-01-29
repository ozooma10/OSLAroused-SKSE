#include "RuntimeEvents.h"

RE::BSEventNotifyControl RuntimeEvents::OnEquipEvent::ProcessEvent(const RE::TESEquipEvent* equipEvent, RE::BSTEventSource<RE::TESEquipEvent>*)
{
	logger::info("EquipEvent for: {} - Item: {} - Slot: {}", equipEvent->actor->GetDisplayFullName(), equipEvent->baseObject, equipEvent->equipped);

	return RE::BSEventNotifyControl::kContinue;
}
