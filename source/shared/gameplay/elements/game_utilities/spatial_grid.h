#ifndef SHARED_GAMEPLAY_ELEMENTS_SPATIAL_GRID
#define SHARED_GAMEPLAY_ELEMENTS_SPATIAL_GRID

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/game_utilities/collision_handler.h"
#include "shared/gameplay/engine/physics_object.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class SpatialGrid
{
public:
	using CollisionPairs = std::vector<std::pair<PhysicsObject*, PhysicsObject*>>;
	using GridCoords = std::pair<int, int>;
	using AABB = std::pair<Vector2D, Vector2D>; // (bottom-left | top-right)

	SpatialGrid(int nrRows, int nrCols);
	~SpatialGrid();
	
	void AddObjectInGrid(PhysicsObject* gameObject, CollisionGroup collisionGroup);
	void ComputeCollisions();
	void ClearGrid();

private:
	// grid properities
	int nrRows;
	int nrCols;
	Vector2D gridSize;
	Vector2D cellSize;
	Vector2D gridTopLeft;
	Vector2D gridBottomRight;

	// grid definitions
	GridCoords GetCoordsForPoint(Vector2D point) const;
	AABB GetCellCoords(int x, int y) const;

	struct Cell {

		static const int CellBucketSize = 20;

		Cell()
		{
			for (size_t indx = 0; indx < nrCollisionGroups; indx++)
				cellBuckets[indx].reserve(CellBucketSize);
		}

		AABB cellAABB;
		
		using Bucket = std::vector<PhysicsObject*>;
		Bucket cellBuckets[nrCollisionGroups];
	};
	
	// grid data
	Cell* spatialGrid;

	void HandleCollisions(const Cell::Bucket& bucket);
	void HandleCollisions(const Cell::Bucket& firstBucket, const Cell::Bucket& secondBucket);
	void HandleCollisions(PhysicsObject* firstObject, PhysicsObject* secondObject);

	CollisionPairs collisionPairs;
	std::unordered_map <int, std::unordered_set<int>> collisionVerified;

	static const GridCoords NotInGrid;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif //SHARED_GAMEPLAY_ELEMENTS_SPATIAL_GRID