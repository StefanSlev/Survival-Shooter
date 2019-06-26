#ifndef SHARED_GAMEPLAY_ELEMENTS_TERRAIN
#define SHARED_GAMEPLAY_ELEMENTS_TERRAIN

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"

// -------------------------------------------------------------------------------------
// Class that defines the terrain.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Terrain : public WorldObject
{
public:

	enum class PlayAreaAnimation {

		ANIMATE
	};

	Terrain();

	ObjectType GetType() const override { return ObjectType::Terrain; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Terrain) || WorldObject::IsTypeOf(type); }

	void Init();

	Vector2D playingArea;
	Vector2D marginArea;

	WorldObject* playingAreaFence;

protected:
	const int TERRAIN_SIZE_Y = 60;
	static const char* terrainFileName;
	static const char* playAreaAnim;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_TERRAIN