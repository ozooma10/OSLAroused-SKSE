#include "ArousalManager.h"
#include "Serialization.h"
#include "Utilities.h"
#include "Papyrus.h"

using namespace Serialization;

float ArousalManager::GetArousal(RE::Actor* actorRef)
{
	RE::FormID actorFormId = actorRef->formID;
	const auto LastCheckTimeData = LastCheckTimeData::GetSingleton();
	auto lastCheckTime = LastCheckTimeData->GetData(actorFormId);
	float curTime = RE::Calendar::GetSingleton()->GetCurrentGameTime();
	float timePased = curTime - lastCheckTime;

	LastCheckTimeData->SetData(actorFormId, curTime);
	float newArousal = 0.f;
	//If never calc or old, regen
	if (lastCheckTime <= 0.0f || timePased > 3.f) { 
		newArousal = Utilities::GenerateRandomFloat(0.f, 75.f);

		if (lastCheckTime <= 0.0f) {
			float randomMultiplier = Utilities::GenerateRandomFloat(0.75f, 1.25f);
			MultiplierData::GetSingleton()->SetData(actorFormId, randomMultiplier);
		}
	} else {
		float currentArousal = ArousalData::GetSingleton()->GetData(actorFormId);
		float arousalMultiplier = MultiplierData::GetSingleton()->GetData(actorFormId);
		newArousal = (float)(currentArousal + ((timePased * 25.0) * arousalMultiplier));
	}
	
	return SetArousal(actorRef, newArousal);
}

float ArousalManager::SetArousal(RE::Actor* actorRef, float value)
{
	value = std::clamp(value, 0.0f, 100.f);

	ArousalData::GetSingleton()->SetData(actorRef->formID, value);

	if (actorRef->IsPlayerRef()) {
		Papyrus::Events::SendPlayerArousalUpdatedEvent(value);
	}

	return value;
}

void ArousalManager::ModifyArousal(RE::Actor* actorRef, float modValue)
{
	if (modValue > 0) {
		modValue *= MultiplierData::GetSingleton()->GetData(actorRef->formID);
	}

	SetArousal(actorRef, ArousalData::GetSingleton()->GetData(actorRef->formID) + modValue);
}
