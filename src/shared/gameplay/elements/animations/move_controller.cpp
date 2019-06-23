#include "shared/gameplay/elements/animations/move_controller.h"
#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

MoveController::MoveController() : Controller()
{
	movableObject = nullptr;

	Init();
}

void MoveController::Init()
{
	inSync = true;
	delayTimer = 0.f;
	//
	oldDirection = 0.f;
	newDirection = 0.f;

	movementSpeed = 0.f;
}

void MoveController::SetObject(Object * _object)
{
	Controller::SetObject(_object);

	if (object->IsTypeOf(ObjectType::PhysicsObject))
		movableObject = static_cast<PhysicsObject*> (_object);
}

void MoveController::Update(float dt) 
{
	if (!inSync)
	{
		if (delayTimer == moveDelay)
			inSync = true;
		else
		{
			Vector2D direction = Lerp(oldDirection, newDirection, delayTimer / moveDelay);

			direction.SafeNormalize();
			movableObject->rigidbody->velocity = direction * movementSpeed;

			delayTimer = fminf(delayTimer + dt, moveDelay);
		}
	}
	
	Controller::Update(dt);
}

void MoveController::MoveObject(Vector2D moveDirection)
{
	if (inSync)
	{
		oldDirection = movableObject->rigidbody->velocity;
		newDirection = moveDirection;

		inSync = false;
		delayTimer = 0.f;
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE