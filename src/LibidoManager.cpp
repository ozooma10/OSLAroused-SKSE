#include "LibidoManager.h"
#include "SavedDataManager.h"
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

float LibidoManager::GetTotalLibido(RE::Actor* actorRef)
{
	return m_LibidoModifierCache(actorRef) + GetBaseLibido(actorRef);
}

float LibidoManager::GetBaseLibido(RE::Actor* actorRef)
{
	return SavedDataManager::BaseLibidoData::GetSingleton()->GetData(actorRef->formID, 0.f);
}

float LibidoManager::SetBaseLibido(RE::Actor* actorRef, float newVal)
{
	newVal = std::clamp(newVal, 0.f, 100.f);
	
	SavedDataManager::BaseLibidoData::GetSingleton()->SetData(actorRef->formID, newVal);

	return newVal;
}

float LibidoManager::ModifyBaseLibido(RE::Actor* actorRef, float modVal)
{
	float curVal = SavedDataManager::BaseLibidoData::GetSingleton()->GetData(actorRef->formID, 0.f);
	if (modVal == 0.f) {
		return curVal;
	}
	curVal += modVal;

	return SetBaseLibido(actorRef, modVal);
}