#ifndef SHARED_GAMEPLAY_ELEMENTS_GAME_UTILS
#define SHARED_GAMEPLAY_ELEMENTS_GAME_UTILS

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Game Utils
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Object;

class GameUtils
{
public:
	enum class ObjectsState {

		Alive,
		NotAlive,
		All
	};

	using ObjectPredicate = std::function<bool(Object*)>;

	// update the list of objects received, taking a certain action for the ones that need to be ended
	static void UpdateGameObjects(std::vector<Object*>& gameObjects, std::function<void(Object*)> EndGameObject, float deltaTime);
	static void UpdatePoolObjects(std::vector<Object*>& poolObjects, float deltaTime);

	static void DestroyPoolObjects(std::vector<Object*>& poolObjects);
	static void DestroyObjects(std::vector<Object*>& objects);

	static void UpdatePhysics(const std::vector<Object*>& gameObjects, float deltaTime);
	static void UpdateGlobal(const std::vector<Object*>& objects);
	static void Render(const std::vector<Object*>& objects, ObjectsState objectsState);
	static void DebugRender(const std::vector<Object*>& gameObjects, ObjectsState objectsState);

	static void CheckCollision(const std::vector<Object*>& objects, float radiusProc = 1.f);
	static void CheckCollision(const std::vector<Object*>& firstSet, const std::vector<Object*>& secondSet, float firstRadiusProc = 1.f, float secondRadiusProc = 1.f);

	static void CheckCollision(Object* firstObject, Object* secondObject, float firstRadiusProc = 1.f, float secondRadiusProc = 1.f);
	static void GetAliveObjects(const std::vector<Object*>& objects, std::vector<Object*>& aliveObjects);

	static const ObjectPredicate CheckAlive;
	static const ObjectPredicate CheckNotAlive;
	static const ObjectPredicate CheckActive;
	static const ObjectPredicate CheckNotActive;

private:
	GameUtils();
	GameUtils(const GameUtils&) = delete;
	void operator= (const GameUtils&) = delete;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_GAME_UTILS