#ifndef SHARED_GAMEPLAY_ELEMENTS_TRANSFORM_ANIMATION
#define SHARED_GAMEPLAY_ELEMENTS_TRANSFORM_ANIMATION

#include "shared/gameplay/elements/animations/animation_controller.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// This is an animation controller used to make objects
// move animated from a position to another
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class TransformAnimation : public AnimationController
{
public:
	TransformAnimation(Vector2D iniPos, Vector2D finalPos);

	ObjectType GetType() const override { return ObjectType::TransformAnimation; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::TransformAnimation) || AnimationController::IsTypeOf(type); }

	void UpdateAnimation(float dt) override;

	Vector2D iniPos;
	Vector2D finalPos;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_TRANSFORM_ANIMATION