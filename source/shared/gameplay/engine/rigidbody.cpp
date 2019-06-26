#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/object.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

RigidBody::RigidBody(float _mass) : Controller()
{
	Reset();

	mass = _mass;
	dragCoef = 0;
	dampCoef = 0;
	angularDragCoef = 0;
	angularDampCoef = 0;
}

void RigidBody::Init()
{
	mass = 1;
	dragCoef = 0;
	dampCoef = 0;
	angularDragCoef = 0;
	angularDampCoef = 0;

	Reset();

	timedImpulse = 0;
	impulseTimer = 0;
	applyImpulse = false;
}

void RigidBody::Reset()
{
	velocity = 0;
	angularVelocity = 0;
	cumulatedForces = 0;
	cumulatedTorque = 0;
	cumulatedImpulses = 0;
	cumulatedImpulseTorque = 0;
}

void RigidBody::AddForce(Vector2D force, ForceType forceType)
{
	AddForceAtPosition(force, Vector2D(0), forceType);
}

void RigidBody::AddForceAtPosition(Vector2D force, Vector2D forcePosition, ForceType forceType)
{
	float cross = force.Cross(forcePosition);
	float torque = (cross > 0 ? 1 : -1) * forcePosition.Length() * force.Length() * sinf(acosf(force.Dot(forcePosition)));

	switch (forceType)
	{
	case ForceType::FORCE:
		cumulatedForces += force;
		break;
	case ForceType::IMPULSE:
		cumulatedImpulses += force;
		break;
	default:
		break;
	}

	if (torque != 0)
		AddTorque(torque, forceType);
}

void RigidBody::AddImpulseForTime(Vector2D force, float time)
{
	timedImpulse = force;
	impulseTimer = time;
	applyImpulse = true;
}

void RigidBody::AddTorque(float torque, ForceType forceType)
{
	switch (forceType)
	{
	case ForceType::FORCE:
		cumulatedTorque += torque;
		break;
	case ForceType::IMPULSE:
		cumulatedImpulseTorque += torque;
		break;
	default:
		break;
	}
}

void RigidBody::Update(float dt)
{
	if (applyImpulse)
	{
		if (impulseTimer == 0.f)
		{
			applyImpulse = false;
			timedImpulse = 0.f;
			return;
		}

		this->AddForce(timedImpulse, ForceType::IMPULSE);

		impulseTimer = fmaxf(impulseTimer - dt, 0.f);
	}
}

void RigidBody::UpdatePhysics(float dt)
{
	// compute the impact of Impulses & Forces on the speed of the object
	Vector2D velDir = velocity;
	velDir.SafeNormalize();

	// consider a (static) drag that acts to stop the object still for small speeds or not let it move if the applied force is too small
	float curDragCoef = velocity.Length() / dt;
	curDragCoef = (curDragCoef > dragCoef) ? dragCoef : curDragCoef;
	// compute the acceleration, considering applied forces and impulses, as well as drag
	Vector2D linearAcc = cumulatedForces / mass - curDragCoef * velDir;
	// compute the new velocity
	velocity += linearAcc * dt + cumulatedImpulses / mass;

	// same as the above, but for the angular speed
	curDragCoef = (angularVelocity < 0 ? -1 : 1) * angularVelocity / dt;
	curDragCoef = (curDragCoef > dragCoef) ? dragCoef : curDragCoef;
	float angularAcc = cumulatedTorque / mass - angularDragCoef;
	//compute new angular velocity
	angularVelocity += angularAcc * dt + cumulatedImpulseTorque / mass;

	// Update the object's position and rotation
	object->rotation += angularVelocity * dt;
	object->position += velocity * dt;

	// reset impulses
	cumulatedImpulses = 0;
	cumulatedImpulseTorque = 0;

	// apply damping on velocities (both linear and angular) for next frame
	velocity *= (1 - dampCoef);
	angularVelocity *= (1 - angularDampCoef);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE