#include "shared/gameplay/elements/game_utilities/collision_handler.h"
#include "shared/gameplay/elements/game_utilities/spatial_grid.h"
#include "shared/gameplay/elements/game_utilities/game_utils.h"
#include "shared/gameplay/engine/physics_object.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

CollisionHandler::CollisionHandler()
{
	spatialGrid = new SpatialGrid(gridRows, gridCols);
}

CollisionHandler::~CollisionHandler()
{
	if (spatialGrid)
		delete spatialGrid;
}

void CollisionHandler::LoadGameObjects(const std::vector<Object*>& gameObjects, CollisionGroup collisionGroup)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->IsTypeOf(ObjectType::PhysicsObject))
		{
			PhysicsObject* entity = static_cast<PhysicsObject*> (gameObjects[i]);

			if (GameUtils::CheckAlive(entity))
				spatialGrid->AddObjectInGrid(entity, collisionGroup);
		}
	}
}

void CollisionHandler::HandleCollisions()
{
	spatialGrid->ComputeCollisions();
}

void CollisionHandler::Clear()
{
	spatialGrid->ClearGrid();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE