#ifndef SHARED_GAMEPLAY_ELEMENTS_CROSSHAIR
#define SHARED_GAMEPLAY_ELEMENTS_CROSSHAIR

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"

// -------------------------------------------------------------------------------------
// Class that defines the life bar of the player.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Crosshair : public WorldObject
{
public:
	Crosshair(Sprite* sprite = nullptr);

	ObjectType GetType() const override { return ObjectType::Crosshair; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Crosshair) || WorldObject::IsTypeOf(type); }

	void Init();
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_CROSSHAIR