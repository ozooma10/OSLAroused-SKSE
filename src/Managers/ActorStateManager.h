#pragma once
#include "PCH.h"
#include "Utilities/LRUCache.h"

bool IsActorNaked(RE::Actor* actorRef);

class ActorStateManager
{
public:

	static ActorStateManager* GetSingleton()
	{
		static ActorStateManager singleton;
		return &singleton;
	}

	ActorStateManager() :
		m_ActorNakedStateCache(std::function<bool(RE::Actor*)>(IsActorNaked), 100) {}

	bool GetActorNaked(RE::Actor* actorRef);
	void ActorNakedStateChanged(RE::Actor* actorRef, bool newNaked);

	bool GetActorSpectatingNaked(RE::Actor* actorRef);
	void UpdateActorsSpectating(std::vector<RE::Actor*> spectators);

private:

	Utilities::LRUCache<RE::Actor*, bool> m_ActorNakedStateCache;

	std::map<RE::Actor*, float> m_NakedSpectatingMap;
};
