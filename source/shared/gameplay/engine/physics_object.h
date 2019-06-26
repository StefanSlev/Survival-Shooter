#ifndef SHARED_GAMEPLAY_ENGINE_PHYSICS_OBJECT
#define SHARED_GAMEPLAY_ENGINE_PHYSICS_OBJECT

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"

// -------------------------------------------------------------------------------------
// Class that defines a Physics Object. This is the class that defines a dynamic game object.
// On top of the Object, it adds RigidBody dynamics as well as collisions.
// It is used by the default implementations of the Player and Enemy classes.
//
// Note: Override one of the OnCollission callbacks if you need to add gameplay
//	functionality when 2 game objects collide
// Note: It adds a new UpdatePhysics method, that needs to be called after the gameplay Update step
//	is done and after the collisions have been checked
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Sprite;
class RigidBody;
class Collider;

class PhysicsObject : public WorldObject
{
public:
	PhysicsObject(Sprite * sprite = nullptr);
	void Init() override;

	virtual ObjectType GetType() const { return ObjectType::PhysicsObject; }
	virtual bool IsTypeOf(ObjectType type) const { return (type == ObjectType::PhysicsObject) || WorldObject::IsTypeOf(type); }

	virtual void Update(float dt);

	// call this after the gameplay Update has finished
	virtual void UpdatePhysics(float dt);

	// when called and if the objects collide, this will trigger one of the OnCollision callbacks
	void CheckCollision(const PhysicsObject & other);

	// override the OnCollision callback functions to implement on collision functionalities
	virtual void OnCollision(PhysicsObject* pOther) { (void)pOther; }
	virtual void OnCollisionTrigger(PhysicsObject* pOther) { (void)pOther; }

	virtual void DebugRender();

	RigidBody *rigidbody;
	Collider *collider;
	
	Sprite* debugSprite;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_PHYSICS_OBJECT