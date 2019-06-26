#include "shared/network/game_objects_handler.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

void GameObjectsHandler::AddGameObject(gameplay::ObjectType objectType, hermes::network::NetworkObjectInitDataSPtr objectInitData)
{
	UNREFERENCED_PARAMETER(objectType);
	UNREFERENCED_PARAMETER(objectInitData);
}

void GameObjectsHandler::OnGameObjectEnd(gameplay::Object* gameObject)
{
	UNREFERENCED_PARAMETER(gameObject);
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE