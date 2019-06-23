#ifndef SHARED_NETWORK_GAME_OBJECTS_HANDLER
#define SHARED_NETWORK_GAME_OBJECTS_HANDLER

#include "shared/network/network_base.h"
#include "shared/gameplay/engine/object.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class GameObjectsHandler
{
public:
	virtual void AddGameObject(gameplay::ObjectType objectType, hermes::network::NetworkObjectInitDataSPtr objectInitData = nullptr);
	virtual void OnGameObjectEnd(gameplay::Object* gameObject);
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_GAME_OBJECTS_HANDLER