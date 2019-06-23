#include "shared/gameplay/elements/animations/transform_animation.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

TransformAnimation::TransformAnimation(Vector2D iniPos, Vector2D finalPos) : AnimationController()
{
	this->iniPos = iniPos;
	this->finalPos = finalPos;
	animationTotalTime = 0.4f;
}

void TransformAnimation::UpdateAnimation(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	object->position = Lerp(iniPos, finalPos, animationTime);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE