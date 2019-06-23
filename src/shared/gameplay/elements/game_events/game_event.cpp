#include "shared/gameplay/elements/game_events/game_event.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

GameEvent* GameEvent::BuildGameEvent(hermes::network::NetworkEventSPtr networkEvent)
{
	GameEventBuildHelper buildHelper;

	BEGIN_NETWORK_MESSAGE_READING(networkEvent, eventStream)
		buildHelper.SerializeInternal(eventStream);
	END_NETWORK_MESSAGE_READING(networkEvent, eventStream)
	
	GameEventType gameEventType = static_cast<GameEventType> (buildHelper.gameEventTypeIndx);
	
	auto& gameEventFactories = GameEventFactories::gameEventFactories();

	auto it_factories = gameEventFactories.find(gameEventType);

	if (it_factories != gameEventFactories.end())
	{
		auto& gameEventFactory = it_factories->second;
		GameEvent* gameEvent = gameEventFactory();

		BEGIN_NETWORK_MESSAGE_READING(networkEvent, eventStream)
			gameEvent->SerializeInternal(eventStream);
		END_NETWORK_MESSAGE_READING(networkEvent, eventStream)

		return gameEvent;
	}

	return nullptr;
}

// DamageActor Event

DamageActorEvent::DamageActorEvent(int idGenerator, int idTarget, int damage) : GameEvent(GameEventType::DamageActor) {

	this->idGenerator = idGenerator;
	this->idTarget = idTarget;
	this->damage = damage;
}

BulletShotEvent::BulletShotEvent(int idGenerator, Vector2D position, float rotation, BulletType bulletType) : GameEvent(GameEventType::BulletShot)
{
	this->idGenerator = idGenerator;
	this->position = position;
	this->rotation = rotation;
	this->bulletType = bulletType;
}

// ApplyPowerUp Event

//ApplyPowerUpEvent::ApplyPowerUpEvent(int idTrigger, PowerUpType powerUpType) : GameEvent(GameEventType::ApplyPowerUp)
//{
//	this->idTrigger = idTrigger;
//	this->powerUpType = powerUpType;
//}
//
//Message* ApplyPowerUpEvent::BuildNetworkMessage() const
//{
//	Message* gameEventMessage = GameEvent::BuildNetworkMessage();
//	const auto& localToNetwork = SynchronizationService::GetInstance().LocalToNetwork();
//
//	int networkIDTrigger = localToNetwork.GetLinkedID(idTrigger);
//	int powerUpTypeIndx = static_cast<int> (powerUpType);
//
//	gameEventMessage->data << VALUE_AND_NAME(networkIDTrigger);
//	gameEventMessage->data << VALUE_AND_NAME(powerUpTypeIndx);
//
//	return gameEventMessage;
//}
//
//std::string ApplyPowerUpEvent::GetInfo() const
//{
//	std::stringstream stream;
//	stream << GameEvent::GetInfo() << " ";
//	stream << idTrigger << " " << static_cast<int> (powerUpType);
//
//	return stream.str();
//}
//
//void ApplyPowerUpEvent::ProcessNetworkMessage(BaseStream& networkData)
//{
//	GameEvent::ProcessNetworkMessage(networkData);
//
//	const auto& networkToLocal = SynchronizationService::GetInstance().NetworkToLocal();
//
//	int networkIDTrigger = -1, powerUpTypeIndx = -1;
//
//	networkData >> VALUE_AND_NAME(networkIDTrigger);
//	networkData >> VALUE_AND_NAME(powerUpTypeIndx);
//
//	idTrigger = networkToLocal.GetLinkedID(networkIDTrigger);
//	powerUpType = static_cast<PowerUpType> (powerUpTypeIndx);
//}
//
//// RemovePowerUp Event
//
//RemovePowerUpEvent::RemovePowerUpEvent(int idActor, PowerUpType powerUpType) : GameEvent(GameEventType::RemovePowerUp)
//{
//	this->idActor = idActor;
//	this->powerUpType = powerUpType;
//}
//
//Message* RemovePowerUpEvent::BuildNetworkMessage() const
//{
//	Message* gameEventMessage = GameEvent::BuildNetworkMessage();
//	const auto& localToNetwork = SynchronizationService::GetInstance().LocalToNetwork();
//
//	int networkIDActor = localToNetwork.GetLinkedID(idActor);
//	int powerUpTypeIndx = static_cast<int> (powerUpType);
//
//	gameEventMessage->data << VALUE_AND_NAME(networkIDActor);
//	gameEventMessage->data << VALUE_AND_NAME(powerUpTypeIndx);
//
//	return gameEventMessage;
//}
//
//std::string RemovePowerUpEvent::GetInfo() const
//{
//	std::stringstream stream;
//	stream << GameEvent::GetInfo() << " ";
//	stream << idActor << " " << static_cast<int> (powerUpType);
//
//	return stream.str();
//}
//
//void RemovePowerUpEvent::ProcessNetworkMessage(BaseStream& networkData)
//{
//	GameEvent::ProcessNetworkMessage(networkData);
//
//	const auto& networkToLocal = SynchronizationService::GetInstance().NetworkToLocal();
//
//	int networkIDActor = -1, powerUpTypeIndx = -1;
//
//	networkData >> VALUE_AND_NAME(networkIDActor);
//	networkData >> VALUE_AND_NAME(powerUpTypeIndx);
//
//	idActor = networkToLocal.GetLinkedID(networkIDActor);
//	powerUpType = static_cast<PowerUpType> (powerUpTypeIndx);
//}

// PlayerDeath Event

PlayerDeathEvent::PlayerDeathEvent(int idPlayer) : GameEvent(GameEventType::PlayerDeath)
{
	this->idPlayer = idPlayer;
}

//Message* PlayerDeathEvent::BuildNetworkMessage() const
//{
//	Message* gameEventMessage = GameEvent::BuildNetworkMessage();
//	const auto& localToNetwork = SynchronizationService::GetInstance().LocalToNetwork();
//
//	int networkIDPlayer = localToNetwork.GetLinkedID(idPlayer);
//	gameEventMessage->data << VALUE_AND_NAME(networkIDPlayer);
//
//	return gameEventMessage;
//}
//
//std::string PlayerDeathEvent::GetInfo() const
//{
//	std::stringstream stream;
//	stream << GameEvent::GetInfo() << " ";
//	stream << idPlayer;
//
//	return stream.str();
//}
//
//void PlayerDeathEvent::ProcessNetworkMessage(BaseStream& networkData)
//{
//	GameEvent::ProcessNetworkMessage(networkData);
//
//	const auto& networkToLocal = SynchronizationService::GetInstance().NetworkToLocal();
//
//	int networkIDPlayer = -1;
//
//	networkData >> VALUE_AND_NAME(networkIDPlayer);
//	
//	idPlayer = networkToLocal.GetLinkedID(networkIDPlayer);
//}

// PlayerRevival Event

PlayerRevivalEvent::PlayerRevivalEvent(int idPlayer) : GameEvent(GameEventType::PlayerRevival)
{
	this->idPlayer = idPlayer;
}

//Message* PlayerRevivalEvent::BuildNetworkMessage() const
//{
//	Message* gameEventMessage = GameEvent::BuildNetworkMessage();
//	const auto& localToNetwork = SynchronizationService::GetInstance().LocalToNetwork();
//
//	int networkIDPlayer = localToNetwork.GetLinkedID(idPlayer);
//	gameEventMessage->data << VALUE_AND_NAME(networkIDPlayer);
//
//	return gameEventMessage;
//}
//
//std::string PlayerRevivalEvent::GetInfo() const
//{
//	std::stringstream stream;
//	stream << GameEvent::GetInfo() << " ";
//	stream << idPlayer;
//
//	return stream.str();
//}
//
//void PlayerRevivalEvent::ProcessNetworkMessage(BaseStream& networkData)
//{
//	GameEvent::ProcessNetworkMessage(networkData);
//
//	const auto& networkToLocal = SynchronizationService::GetInstance().NetworkToLocal();
//
//	int networkIDPlayer = -1;
//
//	networkData >> VALUE_AND_NAME(networkIDPlayer);
//
//	idPlayer = networkToLocal.GetLinkedID(networkIDPlayer);
//}

// PowerUpSpawn Event

//PowerUpSpawnEvent::PowerUpSpawnEvent(PowerUpType powerUpType, Vector2D position) : GameEvent(GameEventType::PowerUpSpawn)
//{
//	this->powerUpType = powerUpType;
//	this->position = position;
//}
//
//// EnemySpawn Event
//
//EnemySpawnEvent::EnemySpawnEvent(ObjectType enemyType) : GameEvent(GameEventType::EnemySpawn)
//{
//	this->enemyType = enemyType;
//}
//
//// SpiderDeath Event
//
//SpiderDeathEvent::SpiderDeathEvent(int nrChildren, Vector2D position, float rotation) : GameEvent(GameEvent::GameEventType::SpiderDeath)
//{
//	this->nrChildren = nrChildren;
//	this->position = position;
//	this->rotation = rotation;
//}
//
//// SpawnEntity Event
//
//EntitySpawnEvent::EntitySpawnEvent(Object* entity) : GameEvent(GameEvent::GameEventType::EntitySpawn) {
//
//	this->entity = entity;
//}
//
//Message* EntitySpawnEvent::BuildNetworkMessage() const
//{
//	Message* gameEventMessage = GameEvent::BuildNetworkMessage();
//	const auto& localToNetwork = SynchronizationService::GetInstance().LocalToNetwork();
//
//	int entityTypeIndx = static_cast<int> (entity->GetType());
//	int networkEntityID = localToNetwork.GetLinkedID(entity->GetID());
//
//	gameEventMessage->data << VALUE_AND_NAME(entityTypeIndx);
//	gameEventMessage->data << VALUE_AND_NAME(networkEntityID);
//
//	if (entity->IsTypeOf(ObjectType::Enemy))
//	{
//		Vector2D enemyPosition = entity->position;
//		gameEventMessage->data << VALUE_AND_NAME(enemyPosition);
//	}
//	else if (entity->IsTypeOf(ObjectType::PowerUp))
//	{
//		PowerUp* powerUp = static_cast<PowerUp*> (entity);
//		int powerUpTypeIndx = static_cast<int> (powerUp->powerUpType);
//		Vector2D powerUpPosition = powerUp->position;
//
//		gameEventMessage->data << VALUE_AND_NAME(powerUpTypeIndx);
//		gameEventMessage->data << VALUE_AND_NAME(powerUpPosition);
//	}
//
//	return gameEventMessage;
//}
//
//std::string EntitySpawnEvent::GetInfo() const
//{
//	std::stringstream stream;
//	stream << GameEvent::GetInfo() << " ";
//	
//	int entityID = entity->GetID();
//	int networkEntityID = SynchronizationService::GetInstance().LocalToNetwork().GetLinkedID(entityID);
//	stream << entityID << " " << networkEntityID;
//
//	return stream.str();
//}
//
//void EntitySpawnEvent::ProcessNetworkMessage(BaseStream& networkData)
//{
//	GameEvent::ProcessNetworkMessage(networkData);
//	SynchronizationService& syncService = SynchronizationService::GetInstance();
//
//	int entityTypeIndx = -1, networkEntityID = -1;
//
//	networkData >> VALUE_AND_NAME(entityTypeIndx);
//	networkData >> VALUE_AND_NAME(networkEntityID);
//	
//	ObjectType entityType = static_cast<ObjectType> (entityTypeIndx);
//	entity = ObjectPools::GetInstance().GetObjectFromPool(entityType);
//
//	// link for sync
//	if (syncService.IsLinkedToNetwork(entity->GetID()))
//		syncService.UnlinkID(SynchronizationService::IDType::Local, entity->GetID());
//
//	if (syncService.IsLinkedToLocal(networkEntityID))
//		syncService.UnlinkID(SynchronizationService::IDType::Network, networkEntityID);
//
//	SynchronizationService::GetInstance().LinkLocalNetworkIDs(entity->GetID(), networkEntityID);
//
//	if (entity->IsTypeOf(ObjectType::Enemy))
//	{
//		Vector2D enemyPosition = 0;
//		networkData >> VALUE_AND_NAME(enemyPosition);
//		
//		entity->position = enemyPosition;
//	}
//	else if (entity->IsTypeOf(ObjectType::PowerUp))
//	{
//		int powerUpTypeIndx = -1;
//		Vector2D powerUpPosition = 0;
//
//		networkData >> VALUE_AND_NAME(powerUpTypeIndx);
//		PowerUpType powerUpType = static_cast<PowerUpType> (powerUpTypeIndx);
//
//		networkData >> VALUE_AND_NAME(powerUpPosition);
//
//		static_cast<PowerUp*> (entity)->Load(powerUpType);
//		entity->position = powerUpPosition;
//	}
//}
//
// EndGameObject Event

EndGameObjectEvent::EndGameObjectEvent(int idGameObject) : GameEvent(GameEvent::GameEventType::EndGameObject)
{
	this->idGameObject = idGameObject;
}

//	// check for sync removal
//
//	bool removeSync = true;
//
//	// check for player ending
//	// synchronization for players should be kept even
//	// on player end (they can be revived)
//	
//	{
//		Object* gameObject = Object::FindByID(idGameObject);
//
//		if (gameObject && gameObject->IsTypeOf(ObjectType::Player))
//			removeSync = false;
//	}
//
//	if (removeSync)
//		syncService.UnlinkID(SynchronizationService::IDType::Local, idGameObject);

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE