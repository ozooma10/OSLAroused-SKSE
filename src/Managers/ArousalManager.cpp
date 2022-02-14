#include "ArousalManager.h"
#include "PersistedData.h"
#include "Utilities/Utils.h"
#include "Papyrus.h"
#include "Settings.h"

using namespace PersistedData;

namespace ArousalManager
{
	float GetArousal(RE::Actor* actorRef, bool bUpdateState)
	{
		if (!actorRef) {
			return -2.f;
		}

		RE::FormID actorFormId = actorRef->formID;

		const auto LastCheckTimeData = LastCheckTimeData::GetSingleton();
		auto lastCheckTime = LastCheckTimeData->GetData(actorFormId, 0.f);
		float curTime = RE::Calendar::GetSingleton()->GetCurrentGameTime();
		float gameHoursPassed = (curTime - lastCheckTime) * 24;

		float newArousal = CalculateArousal(actorRef, gameHoursPassed);

		//If set to update state, or we have never checked (last check time is 0), then update the lastchecktime
		if (bUpdateState || lastCheckTime == 0.f) {
			LastCheckTimeData->SetData(actorFormId, curTime);
			SetArousal(actorRef, newArousal);

			LibidoManager::GetSingleton()->UpdateActorLibido(actorRef, gameHoursPassed, newArousal);
		}

		return newArousal;
	}

	float SetArousal(RE::Actor* actorRef, float value)
	{
		value = std::clamp(value, 0.0f, 100.f);

		ArousalData::GetSingleton()->SetData(actorRef->formID, value);

		Papyrus::Events::SendActorArousalUpdatedEvent(actorRef, value);

		return value;
	}

	float ModifyArousal(RE::Actor* , float modValue)
	{
		//logger::trace("ModifyArousal: {}  by {}", actorRef->GetDisplayFullName(), modValue);

		if (modValue > 0) {
			//modValue *= MultiplierData::GetSingleton()->GetData(actorRef->formID, Settings::GetSingleton()->GetDefaultArousalMultiplier());
		}

		//If we are in sl mode operate on exposure rather then arousal
		//const auto lastCheckTime = LastCheckTimeData::GetSingleton()->GetData(actorRef->formID, 0.f);
		//modValue += GetSexlabExposure(actorRef, RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastCheckTime, false);

		return 0.f;
		//return SetArousal(actorRef, modValue);
	}

	float CalculateArousal(RE::Actor* actorRef, float gameHoursPassed)
	{
		float currentArousal = ArousalData::GetSingleton()->GetData(actorRef->formID, -2.f);
		
		//If never calculated, regen
		if (currentArousal < -1) {
			currentArousal = Utilities::GenerateRandomFloat(10.f, 50.f);
		}

		float currentArousalBaseline = LibidoManager::GetSingleton()->GetBaselineArousal(actorRef);

		float epsilon = 0.5f;
		float t = 1.f - std::pow(epsilon, gameHoursPassed);
		float newArousal = std::lerp(currentArousal, currentArousalBaseline, t);
		//logger::trace("CalculateArousal: from: {} newArousal {} Diff: {}  t: {}", currentArousal, newArousal, newArousal - currentArousal, t);

		return newArousal;
	}
}
