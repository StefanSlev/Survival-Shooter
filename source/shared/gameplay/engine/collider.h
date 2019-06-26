#ifndef SHARED_GAMEPLAY_ENGINE_COLLIDER
#define SHARED_GAMEPLAY_ENGINE_COLLIDER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/controller.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// Class that is used to extend an Object's functionality, and make it collidable
// It is automatically attached to Physical Objects, as it is part of its basic funtionality
//
// Note: a Collider can provide a physical collision and/or trigger an event on collision
// Note: the Collider needs a RigidBody to model a realistic collision response
// Note: use collision groups to ignore collisions between certain groups of objects
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Object;

class Collider : public Controller
{
public:
	Collider();

	virtual void SetObject(Object * _object);

	void ComputeDefaultRadius();

	void CheckCollision(const Collider & other);
	void SetAsTrigger(bool enabled) { isTrigger = enabled; }

	// radius of the collider; defaults to the radius of the circumscribed circle of the object (which is a rectangle)
	float radius;
	// trigger colliders just inform about a collision, do not try to resolve it; defaults to false
	bool isTrigger;
	// you can filter collisions with the help of the collision group (objects with the same collision group do not collide); defaults to -1, which means no collison group
	int collisionGroup;

	virtual void Init();

	bool CircleVsCircle(const Collider & other, float & penetration, Vector2D & normal);

	// the rectangle needs to be axis-aligned
	bool CircleVsRectangle(Vector2D bottomLeft, Vector2D topRight);

protected:
	void ResolveCollision(const Collider & other, const float penetration, const Vector2D & normal);
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_COLLIDER