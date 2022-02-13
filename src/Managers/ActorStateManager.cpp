#include "ActorStateManager.h"
#include "Utilities/Utils.h"
#include "Papyrus.h"
#include "Managers/LibidoManager.h"

bool IsActorNaked(RE::Actor* actorRef)
{
	return Utilities::Actor::IsNaked(actorRef);
}

bool ActorStateManager::GetActorNaked(RE::Actor* actorRef)
{
	return m_ActorNakedStateCache(actorRef);
}

void ActorStateManager::ActorNakedStateChanged(RE::Actor* actorRef, bool newNaked)
{
	m_ActorNakedStateCache.UpdateItem(actorRef, newNaked);
	Papyrus::Events::SendActorNakedUpdatedEvent(actorRef, newNaked);
	
	//Actor Naked updated so remove libido cache entry to force refresh on next fetch
	LibidoManager::GetSingleton()->ActorLibidoModifiersUpdated(actorRef);
}

bool ActorStateManager::GetActorSpectatingNaked(RE::Actor* actorRef)
{
	if (const auto lastViewedGameTime = m_NakedSpectatingMap[actorRef]) {
		//@TODO: Calculate time based off global update cycle [not just 0.72 game hours]
		if (RE::Calendar::GetSingleton()->GetCurrentGameTime() - lastViewedGameTime < 0.03f) {
			return true;
		}
	}
	return false;
}

void ActorStateManager::UpdateActorsSpectating(std::vector<RE::Actor*> spectators)
{
	float currentTime = RE::Calendar::GetSingleton()->GetCurrentGameTime();
	for (const auto spectator : spectators) {
		m_NakedSpectatingMap[spectator] = currentTime;
	}
}
