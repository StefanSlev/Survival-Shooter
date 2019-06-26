#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/sprite.h"
#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/utils.h"
#include "shared/gameplay/engine/resource_manager.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

PhysicsObject::PhysicsObject(Sprite *sprite) : WorldObject(sprite)
{
	collider = static_cast<Collider*>(AddController(new Collider()));
	rigidbody = static_cast<RigidBody*>(AddController(new RigidBody()));
	debugSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, "Data\\Images\\circle.png");
}

void PhysicsObject::Init()
{
	WorldObject::Init();

	rigidbody->Init();
	collider->Init();
}

void PhysicsObject::Update(float dt)
{
	if (!active)
		return;

	rigidbody->Update(dt);

	WorldObject::Update(dt);
}

void PhysicsObject::UpdatePhysics(float dt)
{
	if (!IsAlive())
		return;

	if (rigidbody)
		rigidbody->UpdatePhysics(dt);
}

void PhysicsObject::CheckCollision(const PhysicsObject & other)
{
	if (!IsAlive() || !other.IsAlive())
		return;

	if (collider->collisionGroup != -1 && collider->collisionGroup == other.collider->collisionGroup)
		return;

	collider->CheckCollision(*other.collider);
}

void PhysicsObject::DebugRender()
{
	if (!visible)
		return;

	if (debugSprite)
	{
		Vector2D spritePos;
		Vector2D spriteSize;
		float spriteRotation;
		Camera2D::g_pActiveCamera->WorldSpaceToScreenSpace(this, spritePos, spriteSize, spriteRotation);

		spriteSize = collider->radius * 2.f;
		spriteSize *= global.scale;
		spriteSize *= Camera2D::g_pActiveCamera->screenToWorldScale;
		spriteSize /= Camera2D::g_pActiveCamera->scale;

		// draw the Sprite
		debugSprite->Draw(spritePos, spriteSize, spriteRotation, sColor(color).Mask(global.colorMask), 0, 1);
	}

	Object::DebugRender();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE