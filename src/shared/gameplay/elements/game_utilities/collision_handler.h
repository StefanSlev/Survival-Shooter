#ifndef SHARED_GAMEPLAY_ELEMENTS_COLLISION_HANDLER
#define SHARED_GAMEPLAY_ELEMENTS_COLLISION_HANDLER

#include "shared/gameplay/gameplay_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

// -------------------------------------------------------------------------------------
//	CollisionHandler
// -------------------------------------------------------------------------------------

class SpatialGrid;
class Object;

class CollisionHandler
{
public:
	enum CollisionGroup {

		Player,
		Enemy,
		Bullet,
		Explosion,
		PowerUp
	};

	CollisionHandler();
	~CollisionHandler();

	void LoadGameObjects(const std::vector<Object*>& gameObjects, CollisionGroup collisionGroup);
	void HandleCollisions();
	void Clear();

private:
	SpatialGrid* spatialGrid;

	static const int gridRows = 15;
	static const int gridCols = 21;
};

// defining collision groups
using CollisionGroup = CollisionHandler::CollisionGroup;

const CollisionGroup collisionGroups[] = {
									
	CollisionGroup::Player,
	CollisionGroup::Enemy,
	CollisionGroup::Bullet,
	CollisionGroup::Explosion,
	CollisionGroup::PowerUp
};

const size_t nrCollisionGroups = sizeof(collisionGroups) / sizeof(collisionGroups[0]);

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_COLLISION_HANDLER