#include "RuntimeEvents.h"
#include "Settings.h"
#include "Managers/ArousalManager.h"
#include "Managers/LibidoManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ActorStateManager.h"
#include "Papyrus.h"

#include "Integrations/DevicesIntegration.h"

#include "Utilities/Utils.h"

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

	//First we want to check for nudity status (And send event if changed)
	if (equipmentForm->IsArmor()) {
		const auto armor = equipmentForm->As<RE::TESObjectARMO>();
		if (armor && armor->HasPartOf(RE::BGSBipedObjectForm::BipedObjectSlot::kBody)) {
			//This is body armor so send Change of naked state based on if equipped or not
			ActorStateManager::GetSingleton()->ActorNakedStateChanged(static_cast<RE::Actor*>(equipEvent->actor.get()), !equipEvent->equipped);
		}

		//Changed equipped armor so update devices
		DevicesIntegration::GetSingleton()->ActiveEquipmentChanged(static_cast<RE::Actor*>(equipEvent->actor.get()), equipmentForm, equipEvent->equipped);
	}

	return RE::BSEventNotifyControl::kContinue;
}

std::vector<RE::Actor*> GetNakedActorsInCell(RE::Actor* source);

std::vector<RE::Actor*> GetNearbySpectatingActors(RE::Actor* source, float radius);

void HandleAdultScenes(std::vector<SceneManager::SceneData> activeScenes, float )
{
	float scanDistance = Settings::GetSingleton()->GetScanDistance();
	
	for (const auto scene : activeScenes) {
		if (scene.Participants.size() <= 0) {
			logger::warn("HandleAdultScenes: Skipping sceneid: {} no participants found", scene.SceneId);
			continue;
		}

		const auto spectators = GetNearbySpectatingActors(scene.Participants[0], scanDistance);
		SceneManager::GetSingleton()->UpdateSceneSpectators(spectators);
	}
}

void WorldChecks::ArousalUpdateLoop()
{
	float curHours = RE::Calendar::GetSingleton()->GetHoursPassed();

	float elapsedGameTimeSinceLastCheck = std::clamp(curHours - WorldChecks::AurousalUpdateTicker::GetSingleton()->LastUpdatePollGameTime, 0.f, 1.f);
	logger::trace("ArousalUpdateLoop: {} Game Hours have elapsed since last check {}", elapsedGameTimeSinceLastCheck, curHours);

	WorldChecks::AurousalUpdateTicker::GetSingleton()->LastUpdatePollGameTime = curHours;

	if (elapsedGameTimeSinceLastCheck <= 0) {
		return;
	}

	const auto activeScenes = SceneManager::GetSingleton()->GetAllScenes();
	if (activeScenes.size() > 0) {
		HandleAdultScenes(activeScenes, elapsedGameTimeSinceLastCheck);
	}
	
	auto player = RE::PlayerCharacter::GetSingleton();
	if (!player) {
		return;
	}
	
	float scanDistance = Settings::GetSingleton()->GetScanDistance();
	const auto nakedActors = GetNakedActorsInCell(player);
	for (const auto actor : nakedActors) {
		const auto spectators = GetNearbySpectatingActors(actor, scanDistance);
		ActorStateManager::GetSingleton()->UpdateActorsSpectating(spectators);
	}
}

std::vector<RE::Actor*> GetNakedActorsInCell(RE::Actor* source)
{
	std::vector<RE::Actor*> nakedActors;

	if (!source || !source->parentCell) {
		logger::warn("GetNakedActorsInCell - source can not be null");
		return nakedActors;
	}

	float scanDistance = Settings::GetSingleton()->GetScanDistance();
	const auto actorStateManager = ActorStateManager::GetSingleton();

	Utilities::World::ForEachReferenceInRange(source, scanDistance, [&](RE::TESObjectREFR& ref) {
		auto refBase = ref.GetBaseObject();
		auto actor = ref.As<RE::Actor>();
		if (actor && (ref.Is(RE::FormType::NPC) || (refBase && refBase->Is(RE::FormType::NPC)))) {
			if(actorStateManager->IsHumanoidActor(actor) && actorStateManager->GetActorNaked(actor)) {
				//If Actor is naked
				nakedActors.push_back(actor);
			}
		}
		return true;
	});

	return nakedActors;
}

std::vector<RE::Actor*> GetNearbySpectatingActors(RE::Actor* source, float radius)
{
	std::vector<RE::Actor*> nearbyActors;

	if (!source || !source->parentCell) {
		logger::warn("GetNearbySpectatingActors - source can not be null");
		return nearbyActors;
	}

	//OAroused algo. Anyone nearer than force distance will have there arousal modified [0.125 is 1/8th]
	float forceDetectDistance = radius * 0.125f;

	//Square distances since we check against squared dist
	forceDetectDistance *= forceDetectDistance; 
	radius *= radius;

	const auto sourceLocation = source->GetPosition();
	Utilities::World::ForEachReferenceInRange(source, radius, [&](RE::TESObjectREFR& ref) {
		auto refBase = ref.GetBaseObject();
		auto actor = ref.As<RE::Actor>();
		if (actor && actor != source && (ref.Is(RE::FormType::NPC) || (refBase && refBase->Is(RE::FormType::NPC)))) {
			//If Actor is super close or detects the source, increase arousal
			if (sourceLocation.GetSquaredDistance(ref.GetPosition()) < forceDetectDistance || actor->RequestDetectionLevel(source, RE::DETECTION_PRIORITY::kNormal) > 0) {
				nearbyActors.push_back(actor);
			}
		}
		return true;
	});

	return nearbyActors;
}
