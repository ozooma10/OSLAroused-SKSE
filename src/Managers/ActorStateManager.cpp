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
