#include "ArousalManager.h"
#include "SavedDataManager.h"
#include "Utilities.h"
#include "Papyrus.h"
#include "Settings.h"

using namespace SavedDataManager;

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
		float timePassed = curTime - lastCheckTime;


		//If set to update state, or we have never checked (last check time is 0), then update the lastchecktime
		if (bUpdateState || lastCheckTime == 0.f) {
			LastCheckTimeData->SetData(actorFormId, curTime);
		}

		return GetLastOrgasmArousal(actorRef) + GetSexlabExposure(actorRef, timePassed, bUpdateState);
	}

	float SetArousal(RE::Actor* actorRef, float value)
	{
		value = std::clamp(value, 0.0f, 100.f);

		ArousalData::GetSingleton()->SetData(actorRef->formID, value);

		Papyrus::Events::SendActorArousalUpdatedEvent(actorRef, value);

		return value;
	}

	float ModifyArousal(RE::Actor* actorRef, float modValue)
	{
		logger::trace("ModifyArousal: {}  by {}", actorRef->GetDisplayFullName(), modValue);

		if (modValue > 0) {
			//modValue *= MultiplierData::GetSingleton()->GetData(actorRef->formID, Settings::GetSingleton()->GetDefaultArousalMultiplier());
		}

		//If we are in sl mode operate on exposure rather then arousal
		const auto lastCheckTime = LastCheckTimeData::GetSingleton()->GetData(actorRef->formID, 0.f);
		modValue += GetSexlabExposure(actorRef, RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastCheckTime, false);

		return SetArousal(actorRef, modValue);
	}

	float GetActorTimeRate(RE::Actor* /* actorRef*/, float)  //timeSinceLastOrgasm)
	{
		float decayRate = Settings::GetSingleton()->GetDecayRate();
		if (decayRate <= 0.1) {
			return 10.f;
		}

		//float timeRate = TimeRateData::GetSingleton()->GetData(actorRef->formID, 10.f);
		//timeRate *= std::pow(1.5f, -timeSinceLastOrgasm / decayRate);
		return 0.f;  //timeRate;
	}

	float GetSexlabExposure(RE::Actor* actorRef, float timePassed, bool bUpdateState)
	{
		RE::FormID actorFormId = actorRef->formID;

		float newExposure = ArousalData::GetSingleton()->GetData(actorFormId, -2);

		//If never calculated regen exposure
		if (newExposure < -1) {
			newExposure = Utilities::GenerateRandomFloat(0.0f, 50.f);
		} else {
			float decayRate = Settings::GetSingleton()->GetDecayRate();
			if (decayRate > 0.1f) {
				//Yikes
				newExposure *= std::pow(1.5f, -timePassed / decayRate);
			}
		}

		//We store exposure, arousal is calculated
		if (bUpdateState) {
			return SetArousal(actorRef, newExposure);
		} else {
			return newExposure;
		}
	}

	float GetLastOrgasmArousal(RE::Actor* /* actorRef*/)
	{
		//float lastOrgasmTime = LastOrgasmTimeData::GetSingleton()->GetData(actorRef->formID, 0.f);
		//float daysSinceLastOrgasm = RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastOrgasmTime;

		//return daysSinceLastOrgasm * GetActorTimeRate(actorRef, daysSinceLastOrgasm);
		return 0.f;
	}
}
