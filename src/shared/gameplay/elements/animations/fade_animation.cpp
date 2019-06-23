#include "shared/gameplay/elements/animations/fade_animation.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

FadeAnimation::FadeAnimation() : AnimationController()
{
	visibleMask = sColor(255, 255, 255, 255).GetValue();
	hiddenMask = sColor(0, 255, 255, 255).GetValue();
	animationTotalTime = 0.4f;
}

void FadeAnimation::UpdateAnimation(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	object->colorMask = LerpColor(hiddenMask, visibleMask, animationTime);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE