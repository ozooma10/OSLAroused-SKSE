#include "RuntimeEvents.h"
#include "Settings.h"
#include "ArousalManager.h"
#include "Papyrus.h"

RE::BSEventNotifyControl RuntimeEvents::OnEquipEvent::ProcessEvent(const RE::TESEquipEvent* equipEvent, RE::BSTEventSource<RE::TESEquipEvent>*)
{
	if (!equipEvent || !equipEvent->actor || !equipEvent->baseObject) {
		//Something wrong with this event dont handle
		return RE::BSEventNotifyControl::kContinue;
	}

	auto equipmentForm = RE::TESForm::LookupByID(equipEvent->baseObject);
	if (!equipmentForm) {
		return RE::BSEventNotifyControl::kContinue;
	}

	bool changedBody = false;

	//First we want to check for nudity status (And send event if changed)
	if (equipmentForm->IsArmor()) {
		const auto armor = equipmentForm->As<RE::TESObjectARMO>();
		if (armor && armor->HasPartOf(RE::BGSBipedObjectForm::BipedObjectSlot::kBody)) {
			changedBody = true;
			//This is body armor so send Change of naked state based on if equipped or not
			Papyrus::Events::SendActorNakedUpdatedEvent(static_cast<RE::Actor*>(equipEvent->actor.get()), !equipEvent->equipped);
		}
	}

	//Now we check if we should start the naked player poller (only do this if this event is tied to player and they changed body slot)
	
	//We want to check for case of player removing body armor to start poller
	if (!changedBody || !equipEvent->actor->IsPlayerRef()) {
		return RE::BSEventNotifyControl::kContinue;
	}

	//If nudity check No need to check for armor events
	if (!Settings::GetSingleton()->GetPlayerNudityCheckEnabled()) {
		return RE::BSEventNotifyControl::kContinue;
	}

	//If you have uneequipped body armor, start the naked poller if not already started
	if (!equipEvent->equipped) {
		logger::info("YOU HAVE UNEQUIPPED BODY SLOT: {}", equipEvent->equipped);
		WorldChecks::NakedArousalTicker::GetSingleton()->Start();
	}

	return RE::BSEventNotifyControl::kContinue;
}

void ModifyArousalOfSpecators(RE::Actor* source, float radius, float arousalMod);

void WorldChecks::PlayerNakedUpdateLoop()
{
	//If nudity check not enabled return
	if (!Settings::GetSingleton()->GetPlayerNudityCheckEnabled()) {
		return;
	}

	//If player in sex scene, return (Handled in the Sex Framework Adapter)
	if (Settings::GetSingleton()->GetPlayerInSexScene()) {
		return;
	}

	float curHours = RE::Calendar::GetSingleton()->GetHoursPassed();

	float elapsedGameTimeSinceLastCheck = std::clamp(curHours - WorldChecks::NakedArousalTicker::GetSingleton()->LastNakedPollGameTime, 0.f, 1.f);
	//logger::trace("PlayerNakedUpdateLoop: {} Game Hours have elapsed since last check", elapsedGameTimeSinceLastCheck);

	//wait until some percievable amount of time has passed
	if (elapsedGameTimeSinceLastCheck <= 0.025) {
		return;
	}

	WorldChecks::NakedArousalTicker::GetSingleton()->LastNakedPollGameTime = curHours;

	auto player = RE::PlayerCharacter::GetSingleton();
	//Check if player is naked (no chest)
	if (!player || player->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody) != nullptr) 
	{
		//Player wearing armor, dont check
		logger::trace("PlayerNakedUpdateLoop Stopped because Armor Worn");
		WorldChecks::NakedArousalTicker::GetSingleton()->Stop();
		return;
	}

	//Same dist from OAroused
	//@TODO: Make configurable?
	const float ScanDistance = 5120.f;
	float arousalMod = Settings::GetSingleton()->GetHourlyNudityArousalModifier() * elapsedGameTimeSinceLastCheck;

	//Increase the arousal of any nearby npcs
	ModifyArousalOfSpecators(player, ScanDistance, arousalMod);
}

void ModifyArousalOfSpecators(RE::Actor* source, float radius, float arousalMod)
{
	//logger::trace("ModifyArousalOfSpecators: Modifying Suroundings of {}  within {}  by {}", source->GetDisplayFullName(), radius, arousalMod);
	if (!source || !source->parentCell) {
		logger::warn("ModifyArousalOfSpecators - source can not be null");
		return;
	}

	//OAroused algo. Anyone nearer than force distance will have there arousal modified
	float forceDetectDistance = radius / 8;

	//Square distances since we check against squared dist
	forceDetectDistance *= forceDetectDistance; 
	radius *= radius;

	auto sourceLocation = source->GetPosition();
	source->parentCell->ForEachReferenceInRange(sourceLocation, radius, [&](RE::TESObjectREFR& ref) {
		auto refBase = ref.GetBaseObject();
		auto actor = ref.As<RE::Actor>();
		if (actor && actor != source && (ref.Is(RE::FormType::NPC) || (refBase && refBase->Is(RE::FormType::NPC)))) {
			//If Actor is super close or detects the source, increase arousal
			if (sourceLocation.GetSquaredDistance(ref.GetPosition()) < forceDetectDistance || actor->RequestDetectionLevel(source, RE::DETECTION_PRIORITY::kNormal) > 0) {
				ArousalManager::ModifyArousal(actor, arousalMod);
			}
		}
		return true;
	});
}
