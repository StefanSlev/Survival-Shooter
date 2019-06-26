#include "game_client_logic/game_client_logic.h"

BEGIN_GCL_NAMESPACE

GameClientLogic::GameClientLogic(const yojimbo::Address& address,
								 hermes::communication::CommunicationAdapter& communicationAdapter)
	: GameClient(address, communicationAdapter)
{
	GetSynchronizationService().RegisterNetworkObjects();

	m_localPlayer = nullptr;

	new shared::gameplay::Game();

	auto game = shared::gameplay::Game::g_pGame;
	game->gameEventsQueue->UpdateRights(shared::gameplay::GameEventsQueue::Rights::Slave);
	game->RegisterGameObjectsHandler(this);
}

GameClientLogic::~GameClientLogic()
{
	auto game = shared::gameplay::Game::g_pGame;
	game->UnregisterGameObjectsHandler(this);

	GetSynchronizationService().UnregisterNetworkObjects();
}

void GameClientLogic::Update(double dt)
{
	auto game = shared::gameplay::Game::g_pGame;

	if (IsGameClientState(hermes::communication::GameClientState::NetworkReady))
	{	
		if (game->isLoading && game->resourcesLoaded)
		{
			game->isLoading = false;

			GetLeaderboardService().GetLeaderboard();
			CreateLocalPlayer();
		}
	}

	game->RunFrame(dt);
}

void GameClientLogic::Render()
{
	shared::gameplay::Engine::EngineRender
	(
		shared::gameplay::GameRender,
		shared::gameplay::GameDebugRender
	);
}

void GameClientLogic::CreateLocalPlayer()
{
	auto game = shared::gameplay::Game::g_pGame;
	auto masterObject = GetNetworkFlow().CreateMasterObject(GetObjectTypeIndex(shared::gameplay::ObjectType::Player));

	if (masterObject)
	{
		m_localPlayer = std::static_pointer_cast<shared::network::MasterPlayer> (masterObject);

		int idGameObject = m_localPlayer->GetID();
		auto networkObjectIdValue = m_localPlayer->GetNetworkObjectId().GetValue();

		if (GetSynchronizationService().LinkLocalNetworkIds(idGameObject, networkObjectIdValue))
		{
			auto& gameInput = shared::gameplay::GameInput::GetInstance();
			gameInput.DisconnectKeyboardMouse();

			auto localPlayerDevice = gameInput.ConnectKeyboardMouse();
			m_localPlayer->SetInputDevice(localPlayerDevice);

			GetScoringSystem().RegisterPlayer(idGameObject);
			game->gameHUD->menuPlayerHUD->AddPlayerHUD(m_localPlayer.get());
			game->AddGameObject(m_localPlayer.get());
		}
	}
}

void GameClientLogic::OnGameObjectEnd(shared::gameplay::Object* gameObject)
{
	if (!gameObject || !m_localPlayer || gameObject->GetID() != m_localPlayer->GetID())
		return;

	int idGameObject = m_localPlayer->GetID();
	auto networkObjectIdValue = m_localPlayer->GetNetworkObjectId().GetValue();

	GetNetworkFlow().DestroyMasterObject(networkObjectIdValue);
	GetSynchronizationService().UnlinkLocalId(idGameObject);

	auto game = shared::gameplay::Game::g_pGame;
	game->gameHUD->ResetInfo();
	game->ResetEnemyTargets();

	GetScoringSystem().UnregisterPlayer(idGameObject);
	m_localPlayer.reset();
}

void GameClientLogic::OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent)
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

void GameClientLogic::OnReplicaObjectCreated(std::uint32_t objectType, hermes::network::ReplicaObjectSPtr replicaObject)
{
	if (!replicaObject)
		return;

	auto gameObject = GetGameObjectsConversions().ConvertToGameObject(replicaObject, GetObjectType(objectType));

	if (gameObject)
	{
		int idGameObject = gameObject->GetID();
		auto networkObjectIdValue = replicaObject->GetNetworkObjectId().GetValue();

		// register replica object in cache
		bool cacheRegistered = false;
		auto it = m_replicaObjectsCache.find(networkObjectIdValue);

		if (it == m_replicaObjectsCache.end())
		{
			m_replicaObjectsCache.emplace(networkObjectIdValue, replicaObject);
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
			m_replicaObjectsCache.erase(networkObjectIdValue);
			GetSynchronizationService().UnlinkLocalId(idGameObject);
		}
	}
}

void GameClientLogic::OnReplicaObjectDestroyed(hermes::network::NetworkObjectIdValue objectIdValue)
{
	int idGameObject;
	const auto& networkToLocal = GetSynchronizationService().NetworkToLocal();

	if (networkToLocal.GetLinkedId(objectIdValue, idGameObject))
	{
		auto gameObject = shared::gameplay::Object::FindByID(idGameObject);

		if (gameObject != nullptr)
		{
			// remove from world
			auto game = shared::gameplay::Game::g_pGame;
			game->RemoveGameObject(gameObject);
		}

		m_replicaObjectsCache.erase(objectIdValue);
		GetSynchronizationService().UnlinkLocalId(idGameObject);
	}
}

END_GCL_NAMESPACE