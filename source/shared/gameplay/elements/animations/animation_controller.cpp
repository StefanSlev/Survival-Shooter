#include "shared/gameplay/elements/animations/animation_controller.h"
#include "shared/gameplay/engine/object.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

AnimationController::AnimationController() : Controller()
{
	animationState = AnimationState::FORWARD;
	animate = false;
	animationTime = 0.f;
	animationTotalTime = 1.f;
}

void AnimationController::StartAnimation()
{
	animate = true;
}

bool AnimationController::IsAnimating() const
{
	return animate;
}

void AnimationController::SetAnimationTotalTime(float setAnimationTotalTime)
{
	this->animationTotalTime = setAnimationTotalTime;
}

void AnimationController::SetAnimationState(AnimationState setAnimationState)
{
	switch (setAnimationState)
	{
	case AnimationState::FORWARD:
		this->animationState = AnimationState::FORWARD;
		this->animationTime = 0.f;
		break;
	case AnimationState::BACKWARD:
		this->animationState = AnimationState::BACKWARD;
		animationTime = 1.f;
		break;
	default:
		break;
	}
}

void AnimationController::ToggleState()
{
	switch (animationState)
	{
	case AnimationState::BACKWARD:
		animationState = AnimationState::FORWARD;
		animationTime = 0.f;
		break;
	case AnimationState::FORWARD:
		animationState = AnimationState::BACKWARD;
		animationTime = 1.f;
		break;
	default:
		break;
	}
}

void AnimationController::ToggleAnimation()
{
	if (animate)
		this->OnAnimationEnd();

	switch (animationState)
	{
	case AnimationState::BACKWARD:
		animationState = AnimationState::FORWARD;
		animationTime = animate ? animationTime : 0.f;
		break;
	case AnimationState::FORWARD:
		animationState = AnimationState::BACKWARD;
		animationTime = animate ? animationTime : 1.f;
		break;
	default:
		break;
	}
	animate = true;
}

void AnimationController::Update(float dt)
{
	const float eps = 1e-6f;

	if (animate)
	{
		// check for animation starting
		if ((fabsf(animationTime) < eps && animationState == AnimationState::FORWARD) ||
			(fabsf(animationTime - 1.f) < eps && animationState == AnimationState::BACKWARD))
			this->OnAnimationStart();

		// update the animationTime - [0, 1] based on dt
		animationTime += (animationState == AnimationState::FORWARD ? 1 : -1) * dt / animationTotalTime;
		animationTime = ((animationState == AnimationState::FORWARD) ? fminf(animationTime, 1.f) : fmaxf(animationTime, 0.f));

		this->UpdateAnimation(dt);

		// check for animation ending
		if (animationTime < eps || animationTime - 1.f > -eps)
		{
			animate = false;
			this->OnAnimationEnd();
		}
	}

	Controller::Update(dt);
}

void AnimationController::OnAnimationEnd() {

	if (object->starting)
	{
		object->starting = false;
		object->OnStart();
	}
	else if (object->ending)
	{
		object->ending = false;
		object->OnEnd();
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE