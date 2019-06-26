#include "dedicated_game_server/dedicated_game_server.h"

BEGIN_DGS_NAMESPACE

DedicatedGameServer::DedicatedGameServer(const uint8_t privateKey[],
										 const yojimbo::Address& serverAddress,
										 hermes::communication::DedicatedServerAdapter& dedicatedServerAdapter)
	: DedicatedServer(privateKey, serverAddress, dedicatedServerAdapter)
{
	GetSynchronizationService().RegisterNetworkObjects();

	new shared::gameplay::Game();

	auto game = shared::gameplay::Game::g_pGame;
	game->gameEventsQueue->UpdateRights(shared::gameplay::GameEventsQueue::Rights::Master);
	game->RegisterGameObjectsHandler(this);
}

DedicatedGameServer::~DedicatedGameServer()
{
	auto game = shared::gameplay::Game::g_pGame;
	game->UnregisterGameObjectsHandler(this);

	GetSynchronizationService().UnregisterNetworkObjects();
}

void DedicatedGameServer::Update(double dt)
{
	auto game = shared::gameplay::Game::g_pGame;

	if (game->isLoading && game->resourcesLoaded)
	{
		game->isLoading = false;
	}

	game->RunFrame(dt);
}

void DedicatedGameServer::Render()
{
	shared::gameplay::Engine::EngineRender
	(
		shared::gameplay::GameRender,
		shared::gameplay::GameDebugRender
	);
}

void DedicatedGameServer::OnClientConnected(int clientIndex)
{
	DedicatedServer::OnClientConnected(clientIndex);

	// Join on the fly notifications
	for (auto it = m_replicaPlayersCache.begin(); it != m_replicaPlayersCache.end(); ++it)
	{
		auto networkEvent = CreateReplicaPlayerEvent(it->second);

		if (networkEvent)
			DispatchNetworkMessageToClient(clientIndex, networkEvent);
	}

	for (auto it = m_masterObjectsCache.begin(); it != m_masterObjectsCache.end(); ++it)
	{
		auto networkEvent = CreateMasterObjectEvent(it->second);

		if (networkEvent)
			DispatchNetworkMessageToClient(clientIndex, networkEvent);
	}
}

hermes::network::NetworkEventSPtr DedicatedGameServer::CreateMasterObjectEvent(hermes::network::MasterObjectSPtr masterObject)
{
	if (!masterObject)
		return nullptr;

	auto networkObjectIdValue = masterObject->GetNetworkObjectId().GetValue();

	int idGameObject;
	const auto& networkToLocal = GetSynchronizationService().NetworkToLocal();

	if (networkToLocal.GetLinkedId(networkObjectIdValue, idGameObject))
	{
		auto gameObject = shared::gameplay::Object::FindByID(idGameObject);

		if (gameObject)
		{
			hermes::network::CreateReplicaObjectEventData createReplicaObjectEventData;
			createReplicaObjectEventData.SetObjectType(GetObjectTypeIndex(gameObject->GetType()));
			createReplicaObjectEventData.SetNetworkObjectIdValue(networkObjectIdValue);
			createReplicaObjectEventData.SetHasNetworkObjectInitData(false);

			auto objectInitData = masterObject->GetNetworkObjectInit();

			if (objectInitData)
			{
				auto& objectInitWrapper = createReplicaObjectEventData.GetNetworkObjectInitWrapper();

				BEGIN_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)
					objectInitData->SerializeInternal(initStream);
				END_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)

				createReplicaObjectEventData.SetHasNetworkObjectInitData(true);
			}

			auto networkEvent = std::make_shared<hermes::network::NetworkEvent>();
			auto networkEventType = GetNetworkEventTypeIndex(hermes::network::NetworkEventType::CreateReplicaObject);
			networkEvent->SetNetworkEventType(networkEventType);

			BEGIN_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)
				createReplicaObjectEventData.SerializeInternal(eventStream);
			END_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)

			return networkEvent;
		}
	}

	return nullptr;
}

hermes::network::NetworkEventSPtr DedicatedGameServer::CreateReplicaPlayerEvent(std::shared_ptr<shared::network::ReplicaPlayer> replicaPlayer)
{
	if (!replicaPlayer)
		return nullptr;

	auto networkObjectIdValue = replicaPlayer->GetNetworkObjectId().GetValue();

	hermes::network::CreateReplicaObjectEventData createReplicaObjectEventData;
	createReplicaObjectEventData.SetObjectType(GetObjectTypeIndex(replicaPlayer->GetType()));
	createReplicaObjectEventData.SetNetworkObjectIdValue(networkObjectIdValue);
	createReplicaObjectEventData.SetHasNetworkObjectInitData(false);

	auto objectInitData = replicaPlayer->GetNetworkObjectInit();

	if (objectInitData)
	{
		auto& objectInitWrapper = createReplicaObjectEventData.GetNetworkObjectInitWrapper();

		BEGIN_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)
			objectInitData->SerializeInternal(initStream);
		END_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)

		createReplicaObjectEventData.SetHasNetworkObjectInitData(true);
	}

	auto networkEvent = std::make_shared<hermes::network::NetworkEvent>();
	auto networkEventType = GetNetworkEventTypeIndex(hermes::network::NetworkEventType::CreateReplicaObject);
	networkEvent->SetNetworkEventType(networkEventType);

	BEGIN_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)
		createReplicaObjectEventData.SerializeInternal(eventStream);
	END_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)

	return networkEvent;
}

void DedicatedGameServer::OnClientDisconnected(int clientIndex)
{
	DedicatedServer::OnClientDisconnected(clientIndex);

	auto it = m_replicaPlayersCache.find(clientIndex);

	if (it != m_replicaPlayersCache.end())
	{
		auto replicaPlayer = it->second;
		auto networkObjectIdValue = replicaPlayer->GetNetworkObjectId().GetValue();
		
		if (GetNetworkRegistry().UnregisterReplicaObject(networkObjectIdValue))
		{
			// forces replica destruction
			hermes::network::DestroyReplicaObjectEventData destroyReplicaObjectEventData;
			destroyReplicaObjectEventData.SetNetworkObjectIdValue(networkObjectIdValue);

			GetNetworkFlow().PushNetworkEvent
			(
				hermes::network::NetworkEventType::DestroyReplicaObject,
				destroyReplicaObjectEventData
			);
		}

		OnReplicaObjectDestroyed(networkObjectIdValue);
	}
}

void DedicatedGameServer::AddGameObject(shared::gameplay::ObjectType objectType, hermes::network::NetworkObjectInitDataSPtr objectInitData)
{
	auto masterObject = GetNetworkFlow().CreateMasterObject(GetObjectTypeIndex(objectType), objectInitData);

	if (masterObject)
	{
		auto gameObject = GetGameObjectsConversions().ConvertToGameObject(masterObject, objectType);

		if (gameObject)
		{
			int idGameObject = gameObject->GetID();
			auto networkObjectIdValue = masterObject->GetNetworkObjectId().GetValue();

			// register master object in cache
			bool cacheRegistered = false;
			auto it = m_masterObjectsCache.find(networkObjectIdValue);

			if (it == m_masterObjectsCache.end())
			{
				m_masterObjectsCache.emplace(networkObjectIdValue, masterObject);
				cacheRegistered = true;
			}

			// link object in SynchronizationService
			bool linked = GetSynchronizationService().LinkLocalNetworkIds(idGameObject, networkObjectIdValue);

			if (cacheRegistered && linked)
			{
				// add in world
				auto game = shared::gameplay::Game::g_pGame;
				game->AddGameObject(gameObject);
			}
			else
			{
				// remove bad linkings
				m_masterObjectsCache.erase(networkObjectIdValue);
				GetSynchronizationService().UnlinkLocalId(idGameObject);
			}
		}
	}
}

void DedicatedGameServer::OnGameObjectEnd(shared::gameplay::Object* gameObject)
{
	if (!gameObject)
		return;

	int idGameObject = gameObject->GetID();
	auto networkObjectIdValue = hermes::network::InvalidNetworkObjectIdValue;

	const auto& localToNetwork = GetSynchronizationService().LocalToNetwork();

	if (localToNetwork.GetLinkedId(idGameObject, networkObjectIdValue))
	{
		if (GetNetworkRegistry().IsMasterObjectRegistered(networkObjectIdValue))
		{
			GetNetworkFlow().DestroyMasterObject(networkObjectIdValue);

			m_masterObjectsCache.erase(networkObjectIdValue);
			GetSynchronizationService().UnlinkNetworkId(networkObjectIdValue);
		}
	}
}

void DedicatedGameServer::OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent)
{
	if (!networkEvent)
		return;

	auto game = shared::gameplay::Game::g_pGame;

	switch (networkEvent->GetNetworkEventType())
	{
	case GetNetworkEventTypeIndex(hermes::network::NetworkEventType::NetworkFlow):
	{
		game->OnNetworkEvent(networkEvent);
		break;
	}
	default:
		break;
	}
}

void DedicatedGameServer::OnReplicaObjectCreated(std::uint32_t objectType, hermes::network::ReplicaObjectSPtr replicaObject)
{
	if (!replicaObject || GetObjectType(objectType) != shared::gameplay::ObjectType::Player)
		return;

	auto replicaPlayer = std::static_pointer_cast<shared::network::ReplicaPlayer> (replicaObject);
	
	int idPlayer = replicaPlayer->GetID();
	auto networkObjectId = replicaObject->GetNetworkObjectId();

	// register replica object in cache
	bool cacheRegistered = false;
	int clientIndex = GetClientIndex(networkObjectId.GetNetworkId());

	auto it = m_replicaPlayersCache.find(clientIndex);

	if (it == m_replicaPlayersCache.end())
	{
		m_replicaPlayersCache.emplace(clientIndex, replicaPlayer);
		cacheRegistered = true;
	}

	// link object in SynchronizationService
	bool linked = GetSynchronizationService().LinkLocalNetworkIds(idPlayer, networkObjectId.GetValue());

	if (cacheRegistered && linked)
	{
		// add in world
		auto game = shared::gameplay::Game::g_pGame;
		game->AddGameObject(replicaPlayer.get());

		GetScoringSystem().RegisterPlayer(idPlayer);
	}
	else
	{
		// remove bad linkings
		m_replicaPlayersCache.erase(clientIndex);
		GetSynchronizationService().UnlinkLocalId(replicaPlayer->GetID());
	}
}

void DedicatedGameServer::OnReplicaObjectDestroyed(hermes::network::NetworkObjectIdValue objectIdValue)
{
	int idGameObject;
	const auto& networkToLocal = GetSynchronizationService().NetworkToLocal();

	if (networkToLocal.GetLinkedId(objectIdValue, idGameObject))
	{
		auto gameObject = shared::gameplay::Object::FindByID(idGameObject);

		if (gameObject != nullptr && gameObject->IsTypeOf(shared::gameplay::ObjectType::Player))
		{
			// remove from world
			auto game = shared::gameplay::Game::g_pGame;
			game->RemoveGameObject(gameObject);
			game->ResetEnemyTargets();

			// send player score to leaderboard service
			char buffer[MAX_TEXT_LEN];
			shared::gameplay::Engine::GetUser(buffer, MAX_TEXT_LEN);

			shared::online::LeaderboardEntry entry;
			entry.name = buffer;
			entry.score = GetScoringSystem().GetPlayerScore(idGameObject);

			GetLeaderboardService().AddLeaderboardEntry(entry);
			GetScoringSystem().UnregisterPlayer(idGameObject);

			hermes::network::NetworkObjectId networkObjectId;
			networkObjectId.SetValue(objectIdValue);

			int clientIndex = GetClientIndex(networkObjectId.GetNetworkId());
			m_replicaPlayersCache.erase(clientIndex);
		}
		
		GetSynchronizationService().UnlinkLocalId(idGameObject);
	}
}

END_DGS_NAMESPACE