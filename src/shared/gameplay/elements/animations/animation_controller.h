#ifndef SHARED_GAMEPLAY_ELEMENTS_ANIMATION_CONTROLLER
#define SHARED_GAMEPLAY_ELEMENTS_ANIMATION_CONTROLLER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/controller.h"

// -------------------------------------------------------------------------------------
// This is an abstract class that manages animations in the game
// It contains the main body of an animation, leaving the implementation of
// the animations's update to the animation class that is extending this functionality
// (you need to override void UpdateAnimation(float dt)
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class AnimationController : public Controller
{
public:
	enum class AnimationState
	{
		FORWARD,
		BACKWARD
	};

	AnimationController();

	ObjectType GetType() const override { return ObjectType::AnimationController; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::AnimationController) || Controller::IsTypeOf(type); }

	// the main loop of the animation
	void Update(float dt) override;

	// Override in the actual animation class to specify
	// how the animation should evolve
	virtual void UpdateAnimation(float dt) = 0;
	
	// Useful during an animation to change its state
	// immediately and continue the animation in the new state
	void ToggleAnimation();

	// useful to change the way the animation will go
	void ToggleState();

	// Specify the state the animation will go;
	// Should be used when the animation is not active
	void SetAnimationState(AnimationState setAnimationState);

	void SetAnimationTotalTime(float setAnimationTotalTime);
	void StartAnimation();
	bool IsAnimating() const;

	virtual void OnAnimationStart() {}
	virtual void OnAnimationEnd();

protected:
	AnimationState animationState;
	float animationTime;
	float animationTotalTime;
	bool animate;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_ANIMATION_CONTROLLER