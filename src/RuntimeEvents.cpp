#include "RuntimeEvents.h"

RE::BSEventNotifyControl RuntimeEvents::OnEquipEvent::ProcessEvent(const RE::TESEquipEvent*, RE::BSTEventSource<RE::TESEquipEvent>*)
{
	//logger::info("EquipEvent for: {} - Item: {} - Slot: {}", equipEvent->actor->GetDisplayFullName(), equipEvent->baseObject, equipEvent->equipped);

	return RE::BSEventNotifyControl::kContinue;
}

void WorldChecks::PlayerNakedUpdateLoop()
{
	auto player = RE::PlayerCharacter::GetSingleton();
	//Check if player is naked (no chest)
	if (!player->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody)) 
	{
		logger::info("PlayerNakedUpdateLoop - Not Wearing Body");
	}
}
