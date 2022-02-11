#pragma once

#include "Utilities/LRUCache.h"

float CalculateActorLibidoModifier(RE::Actor* actorFormId);

class LibidoManager
{
public:
	static LibidoManager* GetSingleton()
	{
		static LibidoManager singleton;
		return &singleton;
	}

	LibidoManager() :
		m_LibidoModifierCache(std::function<float(RE::Actor*)>(CalculateActorLibidoModifier), 100) {}
	

	float GetTotalLibido(RE::Actor* actorRef);

	float GetBaseLibido(RE::Actor* actorRef);
	float SetBaseLibido(RE::Actor* actorRef, float newVal);
	float ModifyBaseLibido(RE::Actor* actorRef, float modVal);

private:
	
	//Cache of additional Libido values on top of actors base libido
	Utilities::LRUCache<RE::Actor*, float> m_LibidoModifierCache;
};