#ifndef SHARED_GAMEPLAY_ENGINE_RIGIDBODY
#define SHARED_GAMEPLAY_ENGINE_RIGIDBODY

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/controller.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// Class that is used to extend an Object's functionality, and make it follow dynamics rules
// It is automatically attached to Physical Objects, as it is part of its basic funtionality
//
// Note: Use Drag to oppose the forward movement (as a friction)
// Note: Use Damp to act as a spring decceleration (decrease by [0-1] = [0-100%] of the
//	current speed each frame)
// Note: You can also act directly on velocities if needed; use forces/impulses to act on acceleration
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class RigidBody : public Controller
{
public:
	enum class ForceType : int
	{
		FORCE,			// it continues to act on the body from the moment it is applied
		IMPULSE			// it acts one time on the body, the moment it is applied
	};

	RigidBody(float _mass = 1);
	void Reset();

	void AddForce(Vector2D force, ForceType forceType);
	void AddForceAtPosition(Vector2D force, Vector2D forcePosition, ForceType forceType);
	void AddImpulseForTime(Vector2D force, float time);
	void AddTorque(float torque, ForceType forceType);

	virtual void Update(float dt);

	// Computes the impact of impulses & forces on the speed and angular speed of the object
	// It considers the set drags and damps
	// Moves and rotates the object
	void UpdatePhysics(float dt);

	float mass;

	// use drag to oppose the forward motion
	float dragCoef;
	float angularDragCoef;
	// in Range [0-1], use damp as a spring like decceleration
	float dampCoef;
	float angularDampCoef;

	Vector2D velocity;
	float angularVelocity;

	virtual void Init();

protected:
	Vector2D cumulatedForces;
	float cumulatedTorque;
	Vector2D cumulatedImpulses;
	float cumulatedImpulseTorque;

	// used to continuously add "timedImpulse"
	// until "impulseTimer" gets 0.f

	Vector2D timedImpulse;
	float impulseTimer;
	bool applyImpulse;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_RIGIDBODY