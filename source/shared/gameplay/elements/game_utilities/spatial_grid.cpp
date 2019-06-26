#include "shared/gameplay/elements/game_utilities/spatial_grid.h"
#include "shared/gameplay/elements/game.h"
#include "shared/gameplay/elements/terrain.h"

#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

#define Index(i, j) i * nrCols + j

const SpatialGrid::GridCoords SpatialGrid::NotInGrid = { -1, -1 };

SpatialGrid::SpatialGrid(int nrRows, int nrCols)
{
	this->nrRows = nrRows;
	this->nrCols = nrCols;

	// compute grid properties
	Terrain* terrain = Game::g_pGame->terrain;

	gridSize = terrain->playingAreaFence->size + 2 * terrain->marginArea;
	cellSize = Vector2D(gridSize.x / nrCols, gridSize.y / nrRows);

	gridTopLeft = Vector2D(-0.5f, 0.5f) * gridSize;
	gridBottomRight = Vector2D(0.5f, -0.5f) * gridSize;

	// build grid data
	spatialGrid = new Cell[nrRows * nrCols];

	// precalculate AABB for grid cells
	for (int i = 0; i < nrRows; i++)
		for (int j = 0; j < nrCols; j++)
			spatialGrid[Index(i, j)].cellAABB = GetCellCoords(i, j);
}

SpatialGrid::~SpatialGrid()
{
	delete[] spatialGrid;
}

void SpatialGrid::ComputeCollisions()
{
	for (int i = 0; i < nrRows; i++)
	for (int j = 0; j < nrCols; j++)
	{
		Cell::Bucket* buckets = spatialGrid[Index(i, j)].cellBuckets;

		HandleCollisions(buckets[CollisionGroup::Player]);
		HandleCollisions(buckets[CollisionGroup::Enemy]);

		HandleCollisions(buckets[CollisionGroup::Player], buckets[CollisionGroup::Bullet]);
		HandleCollisions(buckets[CollisionGroup::Player], buckets[CollisionGroup::Explosion]);
		HandleCollisions(buckets[CollisionGroup::Player], buckets[CollisionGroup::PowerUp]);

		HandleCollisions(buckets[CollisionGroup::Enemy], buckets[CollisionGroup::Bullet]);
		HandleCollisions(buckets[CollisionGroup::Enemy], buckets[CollisionGroup::Explosion]);

		HandleCollisions(buckets[CollisionGroup::Player], buckets[CollisionGroup::Enemy]);
	}

	for (unsigned int i = 0; i < collisionPairs.size(); i++)
	{
		PhysicsObject* firstObject = collisionPairs[i].first;
		PhysicsObject* secondObject = collisionPairs[i].second;

		firstObject->CheckCollision(*secondObject);
	}

	collisionPairs.clear();
	collisionVerified.clear();
}

void SpatialGrid::HandleCollisions(const Cell::Bucket& bucket)
{
	if (!bucket.size())
		return;

	for (unsigned int i = 0; i < bucket.size() - 1; i++)
		for (unsigned int j = i + 1; j < bucket.size(); j++)
			HandleCollisions(bucket[i], bucket[j]);
}

void SpatialGrid::HandleCollisions(const Cell::Bucket& firstBucket, const Cell::Bucket& secondBucket)
{
	if (!firstBucket.size() || !secondBucket.size())
		return;

	for (unsigned int i = 0; i < firstBucket.size(); i++)
		for (unsigned int j = 0; j < secondBucket.size(); j++)
			HandleCollisions(firstBucket[i], secondBucket[j]);
}

void SpatialGrid::HandleCollisions(PhysicsObject* firstObject, PhysicsObject* secondObject)
{
	int firstID = firstObject->GetID();
	int secondID = secondObject->GetID();

	auto& firstCollisions = collisionVerified[firstID];

	if (firstCollisions.find(secondID) == firstCollisions.end())
	{
		collisionPairs.emplace_back(firstObject, secondObject);
		firstCollisions.insert(secondID);

		auto& secondCollisions = collisionVerified[secondID];
		secondCollisions.insert(firstID);
	}
}

void SpatialGrid::AddObjectInGrid(PhysicsObject* gameObject, CollisionGroup collisionGroup)
{
	float radius = gameObject->collider->radius;
	Vector2D gameObjectPos = gameObject->position;

	Vector2D topLeft = Vector2D(gameObjectPos.x - radius, gameObjectPos.y + radius);
	Vector2D bottomRight = Vector2D(gameObjectPos.x + radius, gameObjectPos.y - radius);

	GridCoords topLeftCoords = this->GetCoordsForPoint(topLeft);
	GridCoords bottomRightCoords = this->GetCoordsForPoint(bottomRight);

	// if gameObject intersects the grid
	if (topLeftCoords != NotInGrid || bottomRightCoords != NotInGrid)
	{
		// get the sub-matrix of cells in which gameObject can be included
		GridCoords begin = (topLeftCoords != NotInGrid) ? topLeftCoords : GridCoords(0, 0);
		GridCoords end = (bottomRightCoords != NotInGrid) ? bottomRightCoords : GridCoords(nrRows - 1, nrCols - 1);

		for (int i = begin.first; i <= end.first; i++) // left to right
			for (int j = begin.second; j <= end.second; j++) // top to bottom
			{
				Cell& cell = spatialGrid[Index(i, j)];
			
				// if gameObject intersects the cell, add it there
				if (gameObject->collider->CircleVsRectangle(cell.cellAABB.first, cell.cellAABB.second))
					cell.cellBuckets[collisionGroup].push_back(gameObject);
			}
	}
}

void SpatialGrid::ClearGrid()
{
	int nrElems = nrRows * nrCols;

	for (int i = 0; i < nrElems; i++)
	{
		// clear cell buckets
		for (int k = 0; k < nrCollisionGroups; k++)
			spatialGrid[i].cellBuckets[k].clear();
	}
}

SpatialGrid::GridCoords SpatialGrid::GetCoordsForPoint(Vector2D point) const
{
	bool OutOnX = point.x < gridTopLeft.x || point.x > gridBottomRight.x;
	bool OutOnY = point.y < gridBottomRight.y || point.y > gridTopLeft.y;

	GridCoords coords = { -1, -1 };

	if (!OutOnX && !OutOnY)
	{
		float distX = fabsf(point.x - gridTopLeft.x);
		float distY = fabsf(point.y - gridTopLeft.y);

		coords.first = static_cast<int> ((distY * nrRows) / gridSize.y);
		coords.second = static_cast<int> ((distX * nrCols) / gridSize.x);
	}

	return coords;
}

SpatialGrid::AABB SpatialGrid::GetCellCoords(int x, int y) const
{
	std::pair<Vector2D, Vector2D> cell;

	float leftCoord = gridTopLeft.x + y * cellSize.x;
	float rightCoord = leftCoord + cellSize.x;

	float topCoord = gridTopLeft.y - x * cellSize.y;
	float bottomCoord = topCoord - cellSize.y;
	
	cell.first = Vector2D(leftCoord, bottomCoord); // bottom-left
	cell.second = Vector2D(rightCoord, topCoord); // top-right

	return cell;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE