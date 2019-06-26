#include "shared/network/game_objects_conversions.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

bool GameObjectsConversions::UnregisterGameObjectConversion(hermes::network::NetworkObjectType networkObjectType, gameplay::ObjectType objectType)
{
	auto& gameConversions = (networkObjectType == hermes::network::NetworkObjectType::Master) ?
		m_masterConversions : m_replicaConversions;

	auto it = gameConversions.find(objectType);

	if (it != gameConversions.end())
	{
		gameConversions.erase(it);
		return true;
	}

	return false;
}

gameplay::Object* GameObjectsConversions::ConvertToGameObject(hermes::network::NetworkObjectSPtr networkObject, gameplay::ObjectType objectType)
{
	if (!networkObject)
		return nullptr;

	auto& gameConversions = (networkObject->GetNetworkObjectType() == hermes::network::NetworkObjectType::Master) ?
		m_masterConversions : m_replicaConversions;

	auto it = gameConversions.find(objectType);

	if (it != gameConversions.end())
	{
		return it->second(networkObject);
	}

	return nullptr;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE