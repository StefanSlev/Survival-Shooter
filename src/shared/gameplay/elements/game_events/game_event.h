#ifndef SHARED_GAMEPLAY_ELEMENTS_GAME_EVENT
#define SHARED_GAMEPLAY_ELEMENTS_GAME_EVENT

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/event.h"
#include "shared/gameplay/engine/vector2d.h"
#include "shared/gameplay/elements/weapons/weapon_info.h"
#include "shared/network/synchronization_service.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class GameEventBuildHelper : public hermes::network::NetworkFlowEventData
{
public:
	std::uint32_t gameEventTypeIndx;
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		if (Stream::IsReading)
		{
			serialize_uint32(stream, gameEventTypeIndx);
		}
			
		return true;
	}
};

class GameEvent : public Event, public hermes::network::NetworkFlowEventData
{
public:
	enum class GameEventType : std::uint8_t
	{
		DamageActor,
		BulletShot,
		ApplyPowerUp,
		RemovePowerUp,

		PlayerDeath,
		PlayerRevival,

		PowerUpSpawn,
		EnemySpawn,
		SpiderDeath,
		EntitySpawn,

		EndGameObject,
		EndGame
	};
	
	GameEventType gameEventType;
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

	static GameEvent* BuildGameEvent(hermes::network::NetworkEventSPtr networkEvent);

protected:
	GameEvent(GameEventType gameEventType) : gameEventType(gameEventType), Event(Event::EventType::GameEvent) {}

	template <class Stream> bool Serialize(Stream& stream)
	{
		if (Stream::IsWriting)
		{
			std::uint32_t gameEventTypeIndx = static_cast<std::uint32_t> (gameEventType);
			serialize_uint32(stream, gameEventTypeIndx);
		}

		return true;
	}
};

const GameEvent::GameEventType gameEventTypes[] = {
													GameEvent::GameEventType::DamageActor, 
													GameEvent::GameEventType::BulletShot,
													GameEvent::GameEventType::ApplyPowerUp, 
													GameEvent::GameEventType::RemovePowerUp, 
													GameEvent::GameEventType::PowerUpSpawn,
													GameEvent::GameEventType::EnemySpawn,
													GameEvent::GameEventType::SpiderDeath,
													GameEvent::GameEventType::EntitySpawn,
													GameEvent::GameEventType::PlayerDeath,
													GameEvent::GameEventType::PlayerRevival,
													GameEvent::GameEventType::EndGameObject,
													GameEvent::GameEventType::EndGame
};

const size_t nrGameEventTypes = sizeof(gameEventTypes) / sizeof(gameEventTypes[0]);

class DamageActorEvent : public GameEvent
{
public:
	DamageActorEvent(int idGenerator = -1, int idTarget = -1, int damage = 0);

	int idGenerator;
	int idTarget;
	int damage;

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		if (!GameEvent::Serialize(stream))
		{
			return false;
		}

		hermes::network::NetworkObjectIdValue networkIdGenerator{ hermes::network::InvalidNetworkObjectIdValue };
		hermes::network::NetworkObjectIdValue networkIdTarget{ hermes::network::InvalidNetworkObjectIdValue };

		if (Stream::IsWriting)
		{
			const auto& localToNetwork = GetSynchronizationService().LocalToNetwork();

			localToNetwork.GetLinkedId(idGenerator, networkIdGenerator);
			localToNetwork.GetLinkedId(idTarget, networkIdTarget);
		}

		serialize_uint64(stream, networkIdGenerator);
		serialize_uint64(stream, networkIdTarget);

		if (Stream::IsReading)
		{
			const auto& networkToLocal = GetSynchronizationService().NetworkToLocal();

			networkToLocal.GetLinkedId(networkIdGenerator, idGenerator);
			networkToLocal.GetLinkedId(networkIdTarget, idTarget);
		}

		serialize_int(stream, damage, 0, INT_MAX);
		return true;
	}
};

class BulletShotEvent : public GameEvent
{
public:
	BulletShotEvent(int idGenerator = -1, Vector2D position = 0.f, float rotation = 0.f, BulletType bulletType = BulletType::GunBullet);

	int idGenerator;
	Vector2D position;
	float rotation;
	BulletType bulletType;

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		if (!GameEvent::Serialize(stream))
		{
			return false;
		}

		hermes::network::NetworkObjectIdValue networkIdGenerator{ hermes::network::InvalidNetworkObjectIdValue };
		std::uint32_t bulletTypeIndx{ 0 };

		if (Stream::IsWriting)
		{
			const auto& localToNetwork = GetSynchronizationService().LocalToNetwork();
			localToNetwork.GetLinkedId(idGenerator, networkIdGenerator);

			bulletTypeIndx = static_cast<std::uint32_t> (bulletType);
		}

		serialize_uint64(stream, networkIdGenerator);
		serialize_uint32(stream, bulletTypeIndx);

		if (Stream::IsReading)
		{
			const auto& networkToLocal = GetSynchronizationService().NetworkToLocal();
			networkToLocal.GetLinkedId(networkIdGenerator, idGenerator);
			
			bulletType = static_cast<BulletType> (bulletTypeIndx);
		}

		serialize_float(stream, position.x);
		serialize_float(stream, position.y);
		serialize_float(stream, rotation);
		return true;
	}
};

//enum class PowerUpType;
//
//class ApplyPowerUpEvent : public GameEvent
//{
//public:
//	ApplyPowerUpEvent(int idTrigger, PowerUpType powerUpType);
//	ApplyPowerUpEvent() : GameEvent(GameEventType::ApplyPowerUp) {}
//
//	int idTrigger;
//	PowerUpType powerUpType;
//
//	//Message* BuildNetworkMessage() const override;
//	//std::string GetInfo() const override;
//	//void ProcessNetworkMessage(BaseStream& networkData) override;
//};

//class RemovePowerUpEvent : public GameEvent
//{
//public:
//	RemovePowerUpEvent(int idActor, PowerUpType powerUpType);
//	RemovePowerUpEvent() : GameEvent(GameEventType::RemovePowerUp) {}
//
//	int idActor;
//	PowerUpType powerUpType;
//
//	//Message* BuildNetworkMessage() const override;
//	//std::string GetInfo() const override;
//	//void ProcessNetworkMessage(BaseStream& networkData) override;
//};

class PlayerDeathEvent : public GameEvent
{
public:
	PlayerDeathEvent(int idPlayer);
	PlayerDeathEvent() : GameEvent(GameEventType::PlayerDeath) {}

	int idPlayer;
};

class PlayerRevivalEvent : public GameEvent
{
public:
	PlayerRevivalEvent(int idPlayer);
	PlayerRevivalEvent() : GameEvent(GameEventType::PlayerRevival) {}

	int idPlayer;
};

//class PowerUpSpawnEvent : public GameEvent
//{
//public:
//	PowerUpSpawnEvent(PowerUpType powerUpType, Vector2D position);
//	PowerUpSpawnEvent() : GameEvent(GameEventType::PowerUpSpawn) {}
//
//	PowerUpType powerUpType;
//	Vector2D position;
//
//	// event does not generate synchronization message;
//	// see EntitySpawnEvent for that
//	// Message* BuildNetworkMessage() const override { return nullptr; }
//};

//class EnemySpawnEvent : public GameEvent
//{
//public:
//	EnemySpawnEvent(ObjectType enemyType);
//	EnemySpawnEvent() : GameEvent(GameEventType::EnemySpawn) {}
//
//	ObjectType enemyType;
//
//	// event does not generate synchronization message;
//	// see EntitySpawnEvent for that
//	Message* BuildNetworkMessage() const override { return nullptr; }
//};
//
//class SpiderDeathEvent : public GameEvent
//{
//public:
//	SpiderDeathEvent(int nrChildren, Vector2D position, float rotation);
//	SpiderDeathEvent() : GameEvent(GameEventType::SpiderDeath) {}
//
//	int nrChildren;
//	Vector2D position;
//	float rotation;
//
//	// event does not generate synchronization message;
//	// see EntitySpawnEvent for that
//	Message* BuildNetworkMessage() const override { return nullptr; }
//};
//
//class EntitySpawnEvent : public GameEvent
//{
//public:
//	EntitySpawnEvent(Object* entity);
//	EntitySpawnEvent() : GameEvent(GameEventType::EntitySpawn), entity(nullptr) {}
//
//	Object* entity;
//
//	Message* BuildNetworkMessage() const override;
//	std::string GetInfo() const override;
//	void ProcessNetworkMessage(BaseStream& networkData) override;
//};

class EndGameObjectEvent : public GameEvent
{
public:
	EndGameObjectEvent(int idGameObject = -1);

	int idGameObject;

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		if (!GameEvent::Serialize(stream))
		{
			return false;
		}

		hermes::network::NetworkObjectIdValue networkIdGameObject{ hermes::network::InvalidNetworkObjectIdValue };

		if (Stream::IsWriting)
		{
			const auto& localToNetwork = GetSynchronizationService().LocalToNetwork();
			localToNetwork.GetLinkedId(idGameObject, networkIdGameObject);
		}

		serialize_uint64(stream, networkIdGameObject);

		if (Stream::IsReading)
		{
			const auto& networkToLocal = GetSynchronizationService().NetworkToLocal();
			networkToLocal.GetLinkedId(networkIdGameObject, idGameObject);
		}

		return true;
	}
};

class EndGameEvent : public GameEvent
{
public:
	EndGameEvent() : GameEvent(GameEventType::EndGame) {}
};

class GameEventFactories
{
public:
	using GameEventFactory = std::function<GameEvent*(void)>;

	static const std::unordered_map<GameEvent::GameEventType, GameEventFactory>& gameEventFactories()
	{
		const static std::unordered_map<GameEvent::GameEventType, GameEventFactory> gameEventFactories = {

			{GameEvent::GameEventType::DamageActor, []() { return new DamageActorEvent(); }},
			{GameEvent::GameEventType::BulletShot, []() {return new BulletShotEvent(); }},
			//{GameEvent::GameEventType::ApplyPowerUp, []() { return new ApplyPowerUpEvent(); }},
			//{GameEvent::GameEventType::RemovePowerUp, []() { return new RemovePowerUpEvent(); }},
			//{GameEvent::GameEventType::PowerUpSpawn, []() { return new PowerUpSpawnEvent(); }},
			//{GameEvent::GameEventType::EnemySpawn, []() { return new EnemySpawnEvent(); }},
			//{GameEvent::GameEventType::SpiderDeath, []() { return new SpiderDeathEvent(); }},
			//{GameEvent::GameEventType::EntitySpawn, []() { return new EntitySpawnEvent(); }},
			{GameEvent::GameEventType::PlayerDeath, []() { return new PlayerDeathEvent(); }},
			{GameEvent::GameEventType::PlayerRevival, []() { return new PlayerRevivalEvent(); }},
			{GameEvent::GameEventType::EndGameObject, []() { return new EndGameObjectEvent(); }},
			{GameEvent::GameEventType::EndGame, []() { return new EndGameEvent(); }}
		};

		return gameEventFactories;
	}
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_GAME_EVENT