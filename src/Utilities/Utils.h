#pragma once

#include <random>

namespace Utilities
{
	inline float GenerateRandomFloat(float min, float max)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dis(min, max);
		return dis(mt);
	}

	//Keyword logic based off powerof3's CommonLibSSE implementation
	namespace Keywords
	{
		bool AddKeyword(RE::TESForm* form, RE::BGSKeyword* keyword);

		bool RemoveKeyword(RE::TESForm* form, RE::BGSKeyword* keyword);

		void DistributeKeywords();
	}

	namespace Actor
	{
		inline bool IsNaked(RE::Actor* actorRef)
		{
			return actorRef->GetWornArmor(RE::BGSBipedObjectForm::BipedObjectSlot::kBody) == nullptr;
		}
	}
}
#pragma once
