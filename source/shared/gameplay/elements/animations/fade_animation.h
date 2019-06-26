#ifndef SHARED_GAMEPLAY_ELEMENTS_FADE_ANIMATION
#define SHARED_GAMEPLAY_ELEMENTS_FADE_ANIMATION

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/animations/animation_controller.h"

// -------------------------------------------------------------------------------------
// This is an animation controller used to make objects in the game fade out/in
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class FadeAnimation final : public AnimationController
{
public:
	FadeAnimation();

	ObjectType GetType() const override { return ObjectType::FadeAnimation; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::FadeAnimation) || AnimationController::IsTypeOf(type); }

	void UpdateAnimation(float dt) override;

private:
	long hiddenMask;
	long visibleMask;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_FADE_ANIMATION
