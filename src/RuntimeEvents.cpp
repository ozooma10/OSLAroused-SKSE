#include "RuntimeEvents.h"
#include "Settings.h"
#include "ArousalManager.h"
#include "Papyrus.h"
#include "SceneManager.h"

#include "Utilities.h"

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

	return RE::BSEventNotifyControl::kContinue;
}

void ModifyArousalOfSpecators(RE::Actor* source, float radius, float arousalMod, std::vector<RE::Actor*> blacklist = std::vector<RE::Actor*>());

void HandleAdultScenes(std::vector<SceneManager::SceneData> activeScenes, float elapsedGameTime)
{
	float scanDistance = Settings::GetSingleton()->GetScanDistance();
	float participantHourMod = Settings::GetSingleton()->GetHourlySceneParticipantArousalModifier();
	float viewerHourMod = Settings::GetSingleton()->GetHourlySceneViewerArousalModifier();

	for (const auto scene : activeScenes) {
		if (scene.Participants.size() <= 0) {
			logger::warn("HandleAdultScenes: Skipping sceneid: {} no participants found", scene.SceneId);
			continue;
		}

		//@TODO: Be more particular about active animation
		float arousalMod = participantHourMod * elapsedGameTime;
		for (const auto actorRef : scene.Participants) {
			ArousalManager::ModifyArousal(actorRef, arousalMod);
		}

		float viewerMod = viewerHourMod * elapsedGameTime;
		ModifyArousalOfSpecators(scene.Participants[0], scanDistance, viewerMod, scene.Participants);
	}
}

void WorldChecks::ArousalUpdateLoop()
{
	float curHours = RE::Calendar::GetSingleton()->GetHoursPassed();

	float elapsedGameTimeSinceLastCheck = std::clamp(curHours - WorldChecks::AurousalUpdateTicker::GetSingleton()->LastUpdatePollGameTime, 0.f, 1.f);
	//logger::trace("PlayerNakedUpdateLoop: {} Game Hours have elapsed since last check {}", elapsedGameTimeSinceLastCheck, curHours);

	WorldChecks::AurousalUpdateTicker::GetSingleton()->LastUpdatePollGameTime = curHours;

	if (elapsedGameTimeSinceLastCheck <= 0) {
		return;
	}

	//logger::trace("participant Mod is: {}", Settings::GetSingleton()->GetHourlySceneParticipantArousalModifier());
	//logger::trace("Hour Mod is: {}", Settings::GetSingleton()->GetHourlySceneViewerArousalModifier());
	//logger::trace("Naked Mod is: {}", Settings::GetSingleton()->GetHourlyNudityArousalModifier());

	const auto activeScenes = SceneManager::GetSingleton()->GetAllScenes();
	//logger::trace("ArousalUpdateLoop: Currently {} active scenes", activeScenes.size());
	if (activeScenes.size() > 0) {
		HandleAdultScenes(activeScenes, elapsedGameTimeSinceLastCheck);
		//We dont want to run additional nudity checks if any active scenes, as checks and arousal propogation are done in scene handling
		return;
	}

	//If nudity check not enabled return
	if (!Settings::GetSingleton()->GetPlayerNudityCheckEnabled()) {
		return;
	}

	auto player = RE::PlayerCharacter::GetSingleton();

	if (!Utilities::Actor::IsNaked(player)) {
		return;
	}

	//Same dist from OAroused
	//@TODO: Make configurable?
	const float ScanDistance = 5120.f;
	float arousalMod = Settings::GetSingleton()->GetHourlyNudityArousalModifier() * elapsedGameTimeSinceLastCheck;

	//Increase the arousal of any nearby npcs
	ModifyArousalOfSpecators(player, ScanDistance, arousalMod);
}

void ModifyArousalOfSpecators(RE::Actor* source, float radius, float arousalMod, std::vector<RE::Actor*> blacklist)
{
	logger::trace("ModifyArousalOfSpecators: Modifying Suroundings of {}  within {}  by {}", source->GetDisplayFullName(), radius, arousalMod);
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
		if (actor && actor != source && (ref.Is(RE::FormType::NPC) || (refBase && refBase->Is(RE::FormType::NPC))) && std::find(blacklist.begin(), blacklist.end(), actor) == blacklist.end()) {
			//If Actor is super close or detects the source, increase arousal
			if (sourceLocation.GetSquaredDistance(ref.GetPosition()) < forceDetectDistance || actor->RequestDetectionLevel(source, RE::DETECTION_PRIORITY::kNormal) > 0) {
				ArousalManager::ModifyArousal(actor, arousalMod);
			}
		}
		return true;
	});
}
