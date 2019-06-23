#ifndef SHARED_NETWORK_GAME_OBJECTS_CONVERSIONS
#define SHARED_NETWORK_GAME_OBJECTS_CONVERSIONS

#include "shared/network/network_base.h"
#include "shared/gameplay/engine/object.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class GameObjectsConversions : public hermes::system::Singleton<GameObjectsConversions>
{
	SINGLETON_ACCESS(GameObjectsConversions)

public:
	template <class NetworkGameObjectType> 
	bool RegisterGameObjectConversion(hermes::network::NetworkObjectType networkObjectType, gameplay::ObjectType objectType);
	bool UnregisterGameObjectConversion(hermes::network::NetworkObjectType networkObjectType, gameplay::ObjectType objectType);

	gameplay::Object* ConvertToGameObject(hermes::network::NetworkObjectSPtr networkObject, gameplay::ObjectType objectType);

private:
	using GameObjectConversion = std::function<gameplay::Object*(hermes::network::NetworkObjectSPtr)>;

	std::unordered_map<gameplay::ObjectType, GameObjectConversion> m_masterConversions;
	std::unordered_map<gameplay::ObjectType, GameObjectConversion> m_replicaConversions;

	GameObjectsConversions() {}
	~GameObjectsConversions() {}
};

template <class NetworkGameObjectType>
bool GameObjectsConversions::RegisterGameObjectConversion(hermes::network::NetworkObjectType networkObjectType, gameplay::ObjectType objectType)
{
	auto conversion = [](hermes::network::NetworkObjectSPtr networkObject) -> gameplay::Object* 
	{
		if (networkObject)
		{
			auto networkGameObject = std::static_pointer_cast<NetworkGameObjectType> (networkObject);
			return static_cast<gameplay::Object*> (networkGameObject.get());
		}

		return nullptr;
	};

	auto& gameConversions = (networkObjectType == hermes::network::NetworkObjectType::Master) ?
		m_masterConversions : m_replicaConversions;

	auto it = gameConversions.find(objectType);

	if (it == gameConversions.end())
	{
		gameConversions.emplace(objectType, conversion);
		return true;
	}

	return false;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#define GetGameObjectsConversions() shared::network::GameObjectsConversions::GetInstance()

#endif // SHARED_NETWORK_GAME_OBJECTS_CONVERSIONS