#include "LibidoManager.h"
#include "PersistedData.h"
#include "Utilities/Utils.h"

float CalculateActorLibidoModifier(RE::Actor* actorRef)
{
	//Check if in scene
	//Check if near scene
	//Check Clothing
	//Check Devices

	float libidoModifier = 0.f;
	if (Utilities::Actor::IsNaked(actorRef))
	{
		libidoModifier += 50;
	}

	return libidoModifier;
}

float LibidoManager::UpdateActorLibido(RE::Actor* actorRef, float gameHoursPassed, float targetLibido)
{
	//Move base libido towards targetlibido
	float epsilon = 0.9f;
	float currentVal = GetBaseLibido(actorRef);

	//After 1 game hour, distance from curent to target is 10% closer 
	float t = 1.f - pow(epsilon, gameHoursPassed);
	float newVal = std::lerp(currentVal, targetLibido, t);
	logger::trace("UpdateActorLibido: Lerped MOd from {} to {} DIFF: {}  t: {}", currentVal, newVal, newVal - currentVal, t);

	return SetBaseLibido(actorRef, newVal);
}

float LibidoManager::GetBaselineArousal(RE::Actor* actorRef)
{
	return m_LibidoModifierCache(actorRef) + GetBaseLibido(actorRef);
}

float LibidoManager::GetBaseLibido(RE::Actor* actorRef)
{
	return PersistedData::BaseLibidoData::GetSingleton()->GetData(actorRef->formID, 0.f);
}

float LibidoManager::SetBaseLibido(RE::Actor* actorRef, float newVal)
{
	newVal = std::clamp(newVal, 0.f, 100.f);
	
	PersistedData::BaseLibidoData::GetSingleton()->SetData(actorRef->formID, newVal);

	return newVal;
}

float LibidoManager::ModifyBaseLibido(RE::Actor* actorRef, float modVal)
{
	float curVal = PersistedData::BaseLibidoData::GetSingleton()->GetData(actorRef->formID, 0.f);
	if (modVal == 0.f) {
		return curVal;
	}
	curVal += modVal;

	return SetBaseLibido(actorRef, modVal);
}

void LibidoManager::ActorLibidoModifiersUpdated(RE::Actor* actorRef)
{
	m_LibidoModifierCache.PurgeItem(actorRef);
}
