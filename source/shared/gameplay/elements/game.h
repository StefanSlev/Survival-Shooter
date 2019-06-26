#ifndef SHARED_GAMEPLAY_ELEMENTS_GAME
#define SHARED_GAMEPLAY_ELEMENTS_GAME

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/observer.h"
#include "shared/gameplay/elements/actors/enemy_info.h"
#include "shared/network/game_objects_handler.h"

// -------------------------------------------------------------------------------------
// This is the class that manages and distributes the game logic.
// It contains the basic loop of the game, that is run at constant intervals and constitues the game frame  (RunFrame).
// Each dT time interval the game logic is updated, then the physics, and finally everything is drawn on the screen.
// The Game is the one that manages (or distributes the management of) all the entities from the game,
//	like game objects and menu items.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

void GameRender();
void GameDebugRender();

class Terrain;
class CollisionHandler;
class CameraController;

class Player;
struct InputDevice;

class GameEventsQueue;
class GameHUD;
class MenuItem;

class Game : public Object, public Observer
{
public:
	static Game* g_pGame;

	enum class GameState {

		Loading,
		InGame,
		GameOver
	};

	ObjectType GetType() const override { return ObjectType::Game; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Game) || Object::IsTypeOf(type); }

	Game();
	~Game() override;

	void LoadResources();

	void GamePreInit();
	void GameInit();
	void GameCleanup();

	void RunFrame(float dt);
	void Update(float dt) override;
	void UpdatePhysics(float dt);
	void UpdateGlobal() override;

	void Render() override;
	void DebugRender() override;

	float deltaTime;
	
	// Game properties
	GameState gameState;
	GameEventsQueue* gameEventsQueue;

	// game handled objects
	Terrain* terrain;
	std::vector<Object*> players;
	std::vector<Object*> enemies;
	std::vector<Object*> bullets;
	std::vector<Object*> explosions;
	std::vector<Object*> powerUps;
	std::vector<Object*> bulletExplosions;
	std::vector<Object*> nukeExplosions;
	
	void AddGameObject(Object* object);
	void RemoveGameObject(Object* object);
	void ResetEnemyTargets();

	GameHUD* gameHUD;

	bool isLoading;
	std::atomic<bool> resourcesLoaded;

	void OnNetworkEvent(hermes::network::NetworkEventSPtr networkEvent);

	bool RegisterGameObjectsHandler(network::GameObjectsHandler* gameObjectsHandler);
	bool UnregisterGameObjectsHandler(network::GameObjectsHandler* gameObjectsHandler);

private:
	CollisionHandler* collisionHandler;
	CameraController* cameraController;

	// menu pages
	Object* menuLeaderboard;
	Object* menuGameOver;
	std::vector<Object*> menuPages;

	// game handled objects
	const std::unordered_map<ObjectType, std::vector<Object*>&> objectSets = {

		{ ObjectType::Player, players},
		{ ObjectType::Enemy, enemies},
		{ ObjectType::Bullet, bullets},
		{ ObjectType::Explosion, explosions},
		{ ObjectType::PowerUp, powerUps},
		{ ObjectType::BulletExplosion, bulletExplosions},
		{ ObjectType::NukeExplosion, nukeExplosions}
	};

	// enemies spawning
	float enemyTimers[nrEnemies];
	float enemySpawnIntervals[nrEnemies];

	std::unordered_map<ObjectType, int> enemiesTypesIndxs =
	{
		{ObjectType::Cyclope, 0},
		{ObjectType::Ghoul, 1},
		{ObjectType::Kamikaze, 2},
		{ObjectType::Spider, 3},
		{ObjectType::Undead, 4},
	};

	network::GameObjectsHandler* m_gameObjectsHandler;

	// loading data
	MenuItem* menuGameLoad;

	void HandleDebug();
	void HandleLoadingUpdate();
	void HandleInGameUpdate();

	void HandleGameObjectsUpdate();
	void HandleGameObjectsDestruction();
	void OnNotify(Event* event);

	static const char* IMAGES_PATH;
	static const char* ANIMS_PATH;
	static const int MaxNrEnemies = 1000;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_GAME