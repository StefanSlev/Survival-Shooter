#include "shared/gameplay/elements/game.h"

//engine
#include "shared/gameplay/engine/input.h"
#include "shared/gameplay/engine/engine.h"
#include "shared/gameplay/engine/object_pools.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/menu_item.h"
#include "shared/gameplay/engine/camera.h"

// animations
#include "shared/gameplay/elements/animations/camera_controller.h"

// bullets
#include "shared/gameplay/elements/bullets/bullet.h"
#include "shared/gameplay/elements/bullets/gauss_bullet.h"
#include "shared/gameplay/elements/bullets/rocket.h"

// fx
#include "shared/gameplay/elements/fx/bullet_explosion.h"
#include "shared/gameplay/elements/fx/rocket_explosion.h"
#include "shared/gameplay/elements/fx/splatter.h"

// game utilities
#include "shared/gameplay/elements/game_utilities/game_input.h"
#include "shared/gameplay/elements/game_utilities/game_utils.h"
#include "shared/gameplay/elements/game_utilities/collision_handler.h"

// game events
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"

//menu pages
#include "shared/gameplay/elements/menu_pages/menu_game_load.h"
#include "shared/gameplay/elements/menu_pages/menu_player_hud.h"
#include "shared/gameplay/elements/menu_pages/game_hud.h"

// terrain
#include "shared/gameplay/elements/terrain.h"

// network
#include "shared/network/master_objects/master_kamikaze.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

void GameRender()
{
	if (Game::g_pGame)
		Game::g_pGame->Render();
}

void GameDebugRender()
{
	if (Game::g_pGame)
		Game::g_pGame->DebugRender();
}

Game* Game::g_pGame = nullptr;
const char* Game::IMAGES_PATH = "Data\\Images";
const char* Game::ANIMS_PATH = "Data\\Anims";

Game::Game() : Object()
{
	if (!g_pGame)
		g_pGame = this;

	gameEventsQueue = new GameEventsQueue();
	gameEventsQueue->GameEventNotify().RegisterObserver(Event::EventType::GameEvent, this);

	gameState = GameState::Loading;
	deltaTime = 0.f;

	terrain = nullptr;
	collisionHandler = nullptr;
	cameraController = nullptr;

	menuLeaderboard = nullptr;
	menuGameOver = nullptr;

	resourcesLoaded = false;
	isLoading = true;

	gameHUD = new GameHUD();
	gameHUD->Deactivate();
	menuPages.push_back(gameHUD);

	menuGameLoad = new MenuGameLoad();
	menuPages.push_back(menuGameLoad);

	std::thread loader(&Game::LoadResources, this);
	loader.detach();
}

Game::~Game()
{
	// this should be called before ClearPools() to have all the objects
	// back in the their corresponding pools
	this->HandleGameObjectsDestruction();
	ObjectPools::GetInstance().ClearPools();

	GameUtils::DestroyObjects(menuPages);

	delete terrain;
	delete collisionHandler;

	gameEventsQueue->GameEventNotify().UnregisterObserver(Event::EventType::GameEvent, this);
	delete gameEventsQueue;
}

void Game::LoadResources()
{
	using namespace std;
	using namespace std::experimental::filesystem;

	try {

		ResourceManager& manager = ResourceManager::GetInstance();

		path imagesPath = IMAGES_PATH;
		path animsPath = ANIMS_PATH;

		recursive_directory_iterator begin_images(imagesPath), end_images;
		recursive_directory_iterator begin_anims(animsPath), end_anims;

		vector<directory_entry> images(begin_images, end_images);

		for (auto it : images)
			manager.LoadSprite(ResourceType::IMAGE, it.path().string());

		vector<directory_entry> anims(begin_anims, end_anims);

		for (auto it : anims)
			manager.LoadSprite(ResourceType::ANIMATION, it.path().string());

		images.clear();
		anims.clear();
	}
	catch (...) {}

	// build object pools
	ObjectPools& objectPools = ObjectPools::GetInstance();
	objectPools.CreateObjectPool(ObjectType::Bullet, CreateObjectFactory(Bullet));
	objectPools.CreateObjectPool(ObjectType::GaussBullet, CreateObjectFactory(GaussBullet));
	objectPools.CreateObjectPool(ObjectType::Rocket, CreateObjectFactory(Rocket));
	objectPools.CreateObjectPool(ObjectType::BulletExplosion, CreateObjectFactory(BulletExplosion));
	objectPools.CreateObjectPool(ObjectType::RocketExplosion, CreateObjectFactory(RocketExplosion));
	objectPools.CreateObjectPool(ObjectType::Splatter, CreateObjectFactory(Splatter));

	this->resourcesLoaded = true;
}

void Game::GamePreInit()
{
	//// create the game over page
	//menuGameOver = new MenuGameOver();
	//menuGameOver->Deactivate();
	//menuPages.push_back(menuGameOver);

	// create the terrain
	terrain = new Terrain();
	collisionHandler = new CollisionHandler();

	// create camera Controller
	cameraController = new CameraController();
	Camera2D::g_pActiveCamera->AddController(cameraController);
}

void Game::GameInit()
{
	gameState = GameState::InGame;
	memset(enemyTimers, 0, sizeof(float) * nrEnemies);

	enemySpawnIntervals[enemiesTypesIndxs[ObjectType::Cyclope]] = 30.f;
	enemySpawnIntervals[enemiesTypesIndxs[ObjectType::Ghoul]] = 20.f;
	enemySpawnIntervals[enemiesTypesIndxs[ObjectType::Kamikaze]] = 15.f;
	enemySpawnIntervals[enemiesTypesIndxs[ObjectType::Spider]] = 10.f;
	enemySpawnIntervals[enemiesTypesIndxs[ObjectType::Undead]] = 5.f;

	cameraController->Init();
}

void Game::GameCleanup()
{
	// reset input devices
	GameInput::GetInstance().ResetDevices();

	gameEventsQueue->UpdateRights(GameEventsQueue::Rights::Master);

	// reset HUD
	 static_cast<GameHUD*>(gameHUD)->ResetInfo();

	// reset camera properties
	Camera2D::g_pActiveCamera->position = 0.f;
	Camera2D::g_pActiveCamera->scale = 1.f;

	this->HandleGameObjectsDestruction();
}

void Game::RunFrame(float dt)
{
	deltaTime = dt;

	this->Update(dt);
	this->UpdatePhysics(dt);
	this->gameEventsQueue->ProcessEvents();

	Camera2D::g_pActiveCamera->Update(dt);
	this->UpdateGlobal();
}

void Game::Update(float dt)
{
	this->HandleDebug();

	switch (gameState)
	{
	case GameState::Loading:
		this->HandleLoadingUpdate();
		break;
	case GameState::InGame:
		this->HandleInGameUpdate();
		break;
	case GameState::GameOver:
		this->HandleGameObjectsUpdate();
		break;
	default:
		break;
	}

	// update menu pages
	for (Object* menu : menuPages)
		if (menu)
			menu->Update(dt);

	Object::Update(dt);
}

void Game::UpdatePhysics(float dt)
{
	if (gameState != GameState::InGame && gameState != GameState::GameOver)
		return;

	collisionHandler->LoadGameObjects(players, CollisionHandler::CollisionGroup::Player);
	collisionHandler->LoadGameObjects(enemies, CollisionHandler::CollisionGroup::Enemy);
	collisionHandler->LoadGameObjects(bullets, CollisionHandler::CollisionGroup::Bullet);
	collisionHandler->LoadGameObjects(explosions, CollisionHandler::CollisionGroup::Explosion);
	collisionHandler->LoadGameObjects(powerUps, CollisionHandler::CollisionGroup::PowerUp);

	collisionHandler->HandleCollisions();
	collisionHandler->Clear();
	
	// Update rigidbodies (player, enemies and others you might have)
	GameUtils::UpdatePhysics(players, dt);
	GameUtils::UpdatePhysics(enemies, dt);
	GameUtils::UpdatePhysics(bullets, dt);
	GameUtils::UpdatePhysics(explosions, dt);
	GameUtils::UpdatePhysics(powerUps, dt);
}

void Game::UpdateGlobal()
{
	GameUtils::UpdateGlobal(menuPages);
	GameUtils::UpdateGlobal(nukeExplosions);

	if (terrain)
		terrain->UpdateGlobal();

	GameUtils::UpdateGlobal(players);
	GameUtils::UpdateGlobal(enemies);
	GameUtils::UpdateGlobal(bullets);
	GameUtils::UpdateGlobal(explosions);
	GameUtils::UpdateGlobal(bulletExplosions);
	GameUtils::UpdateGlobal(powerUps);

	Object::UpdateGlobal();
}

void Game::Render()
{
	// Render the terrain
	if (terrain)
		terrain->Render();

	GameUtils::Render(enemies, GameUtils::ObjectsState::NotAlive);
	GameUtils::Render(players, GameUtils::ObjectsState::NotAlive);

	GameUtils::Render(powerUps, GameUtils::ObjectsState::All);
	GameUtils::Render(players, GameUtils::ObjectsState::Alive);
	GameUtils::Render(bullets, GameUtils::ObjectsState::All);
	GameUtils::Render(bulletExplosions, GameUtils::ObjectsState::All);
	GameUtils::Render(enemies, GameUtils::ObjectsState::Alive);
	GameUtils::Render(explosions, GameUtils::ObjectsState::All);

	// Render menu pages last, as they are drawn on top of everything else
	GameUtils::Render(menuPages, GameUtils::ObjectsState::All);
	GameUtils::Render(nukeExplosions, GameUtils::ObjectsState::All);

	Object::Render();
}

void Game::DebugRender()
{
	GameUtils::DebugRender(enemies, GameUtils::ObjectsState::NotAlive);
	GameUtils::DebugRender(players, GameUtils::ObjectsState::NotAlive);

	GameUtils::DebugRender(powerUps, GameUtils::ObjectsState::All);
	GameUtils::DebugRender(players, GameUtils::ObjectsState::Alive);
	GameUtils::DebugRender(bullets, GameUtils::ObjectsState::All);
	GameUtils::DebugRender(enemies, GameUtils::ObjectsState::Alive);
	GameUtils::DebugRender(explosions, GameUtils::ObjectsState::All);
	
	Object::DebugRender();
}

void Game::HandleDebug()
{
	// Show / hide debug output on '`' press
	if (Input::IsKeyPressed(VK_OEM_3))
		Engine::g_bShowDebugOutput = !Engine::g_bShowDebugOutput;

	// Show player information on debug
	if (Engine::g_bShowDebugOutput)
	{
		std::stringstream debugStream;

		if (gameState == GameState::InGame)
		{
		}

		Engine::OutputDebugText(debugStream.str().c_str());
	}
}

void Game::HandleLoadingUpdate()
{
	if (gameState != GameState::Loading || !menuGameLoad->IsAlive())
		return;

	if (!isLoading)
	{
		this->GamePreInit();	
		menuGameLoad->End();
	}
}

void Game::HandleInGameUpdate()
{
	if (gameState != GameState::InGame)
		return;

	GameInput& gameInput = GameInput::GetInstance();

	// Check for camera zoom animation
	if (gameInput.CheckCameraToggle())
		cameraController->ToggleAnimation();

	// update stuff
	bool canUpdateGameMode = (gameEventsQueue->GetRights() == GameEventsQueue::Rights::Master);

	if (canUpdateGameMode)
	{
		for (size_t indx = 0; indx < nrEnemies; indx++)
		{
			enemyTimers[indx] += deltaTime * players.size();

			if (enemyTimers[indx] > enemySpawnIntervals[indx])
			{
				enemyTimers[indx] -= enemySpawnIntervals[indx];
				// enemySpawnIntervals[indx] = 1000000000.f;

				if (enemies.size() < MaxNrEnemies && m_gameObjectsHandler)
				{
					m_gameObjectsHandler->AddGameObject(enemyTypes[indx]);
				}
			}
		}
	}

	this->HandleGameObjectsUpdate();
}

void Game::HandleGameObjectsUpdate()
{
	if (terrain)
		terrain->Update(deltaTime);

	// update players
	GameUtils::UpdateGameObjects(players, [this](Object* player) {

		if (m_gameObjectsHandler)
			m_gameObjectsHandler->OnGameObjectEnd(player);

	}, deltaTime);

	// update enemies
	GameUtils::UpdateGameObjects(enemies, [this](Object* enemy) {

		if (m_gameObjectsHandler)
			m_gameObjectsHandler->OnGameObjectEnd(enemy);

	}, deltaTime);

	GameUtils::UpdatePoolObjects(bullets, deltaTime);
	GameUtils::UpdatePoolObjects(explosions, deltaTime);
	GameUtils::UpdatePoolObjects(bulletExplosions, deltaTime);
	GameUtils::UpdatePoolObjects(powerUps, deltaTime);

	GameUtils::UpdateGameObjects(nukeExplosions, [](Object* menuPage) {

		delete menuPage;
	}, deltaTime);
}

void Game::HandleGameObjectsDestruction()
{
	GameUtils::DestroyPoolObjects(enemies);
	GameUtils::DestroyPoolObjects(bullets);
	GameUtils::DestroyPoolObjects(explosions);
	GameUtils::DestroyPoolObjects(powerUps);
	GameUtils::DestroyPoolObjects(bulletExplosions);
	GameUtils::DestroyPoolObjects(players);
	GameUtils::DestroyObjects(nukeExplosions);
}

//
//void Game::HandleGameEnd()
//{
//	if (gameState != GameState::InGame)
//		return;
//
//	if (IsOnlineGameSelected())
//	{
//		if (multiplayerMatch->IsCurrentGameIDHost())
//		{
//			WebSocketMessage* endGameMessage = new WebSocketMessage();
//			endGameMessage->webSocketMessageType = WebSocketMessageType::EndGameMessage;
//
//			WebSocketConnection::GetInstance().SendWebSocketMessage(endGameMessage);
//		}
//
//		if (menuGamePause->IsAlive())
//			menuGamePause->Deactivate();
//	}
//
//	menuGameOver->Start();
//}
//
void Game::AddGameObject(Object* object)
{
	if (!object)
		return;

	for (auto it = objectSets.begin(); it != objectSets.end(); ++it)
	{
		auto& set = it->second;
		ObjectType setObjectType = it->first;

		if (object->IsTypeOf(setObjectType))
		{
			set.push_back(object);
			return;
		}
	}
}

void Game::RemoveGameObject(Object* object)
{
	if (!object)
		return;

	for (auto it = objectSets.begin(); it != objectSets.end(); ++it)
	{
		auto& set = it->second;
		ObjectType setObjectType = it->first;
		
		if (object->IsTypeOf(setObjectType))
		{
			auto object_it = std::find(set.begin(), set.end(), object);

			if (object_it != set.end())
			{
				set.erase(object_it);
			}

			return;
		}
	}
}

void Game::OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent)
{
	bool enteringGame = (gameState == GameState::Loading && !menuGameLoad->IsAlive());
	bool willHandle = (gameState == GameState::InGame || enteringGame);

	if (!willHandle)
		return;

	GameEvent* gameEvent = GameEvent::BuildGameEvent(networkEvent);
	
	if (gameEvent)
	{
		gameEventsQueue->HandleEvent(gameEvent);
		delete gameEvent;
	}
}

void Game::OnNotify(Event* event)
{
	if (!event || event->eventType != Event::EventType::GameEvent)
		return;

	GameEvent* gameEvent = static_cast<GameEvent*> (event);

	GetNetworkFlow().PushNetworkEvent(hermes::network::NetworkEventType::NetworkFlow, *gameEvent);
}

bool Game::RegisterGameObjectsHandler(network::GameObjectsHandler* gameObjectsHandler)
{
	if (!gameObjectsHandler)
		return false;

	if (m_gameObjectsHandler == nullptr)
	{
		m_gameObjectsHandler = gameObjectsHandler;
		return true;
	}

	return false;
}

bool Game::UnregisterGameObjectsHandler(network::GameObjectsHandler* gameObjectsHandler)
{
	if (!gameObjectsHandler)
		return false;

	if (m_gameObjectsHandler == gameObjectsHandler)
	{
		m_gameObjectsHandler = nullptr;
		return true;
	}

	return false;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE