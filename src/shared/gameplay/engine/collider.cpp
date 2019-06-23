#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Collider::Collider() : Controller()
{
	isTrigger = false;
	// set collision group to no group (-1)
	collisionGroup = -1;
}

void Collider::Init()
{
	isTrigger = false;
	collisionGroup = -1;
	radius = 0.f;
}

void Collider::SetObject(Object * _object)
{
	Controller::SetObject(_object);
	ComputeDefaultRadius();
}

void Collider::ComputeDefaultRadius()
{
	radius = fmaxf(object->size.x, object->size.y) / 2;
}

void Collider::CheckCollision(const Collider & other)
{
	if (collisionGroup != -1 && collisionGroup == other.collisionGroup)
		return;

	float penetration;
	Vector2D normal;
	bool collided = CircleVsCircle(other, penetration, normal);

	if (collided) {
		if (isTrigger || other.isTrigger) {
			static_cast<PhysicsObject*>(object)->OnCollisionTrigger(static_cast<PhysicsObject*>(other.object));
			static_cast<PhysicsObject*>(other.object)->OnCollisionTrigger(static_cast<PhysicsObject*>(object));
		}
		else {
			ResolveCollision(other, penetration, normal);
			static_cast<PhysicsObject*>(object)->OnCollision(static_cast<PhysicsObject*>(other.object));
			static_cast<PhysicsObject*>(other.object)->OnCollision(static_cast<PhysicsObject*>(object));
		}
	}
}

bool Collider::CircleVsCircle(const Collider & other, float & penetration, Vector2D & normal)
{
	normal = other.object->position - object->position;
	float rsum = radius + other.radius;

	float dist = normal.LengthSq();
	if (dist > rsum * rsum)
		return false;
	dist = sqrtf(dist);

	if (dist != 0) {
		penetration = rsum - dist;
		normal /= dist;
	}
	else {
		penetration = radius;
		normal = Vector2D(0, 1);
	}

	return true;
}

bool Collider::CircleVsRectangle(Vector2D bottomLeft, Vector2D topRight)
{
	Vector2D circleCenter = object->position;
	Vector2D closestToCircle = 0.f;

	closestToCircle.x = fmaxf(bottomLeft.x, fminf(circleCenter.x, topRight.x));
	closestToCircle.y = fmaxf(bottomLeft.y, fminf(circleCenter.y, topRight.y));

	return DistSq(circleCenter, closestToCircle) < radius * radius;
}

void Collider::ResolveCollision(const Collider &other, const float penetration, const Vector2D & normal)
{
	PhysicsObject* first = static_cast<PhysicsObject*>(object);
	PhysicsObject* second = static_cast<PhysicsObject*>(other.object);

	RigidBody* firstBody = first->rigidbody;
	RigidBody* secondBody = second->rigidbody;

	Vector2D impulse = normal * penetration * ((firstBody->mass * secondBody->mass) / (firstBody->mass + secondBody->mass));

	firstBody->AddForce((-1) * impulse, RigidBody::ForceType::IMPULSE);
	secondBody->AddForce(impulse, RigidBody::ForceType::IMPULSE);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE