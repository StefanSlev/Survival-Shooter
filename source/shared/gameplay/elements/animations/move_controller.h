#ifndef SHARED_GAMEPLAY_ELEMENTS_MOVE_CONTROLLER
#define SHARED_GAMEPLAY_ELEMENTS_MOVE_CONTROLLER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/controller.h"
#include "shared/gameplay/engine/vector2d.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class PhysicsObject;

class MoveController : public Controller
{
public:
	MoveController();

	void Update(float dt) override;
	void Init();

	void UpdateSpeed(float updateMovementSpeed) { this->movementSpeed = fmaxf(updateMovementSpeed, 0.f); }
	void MoveObject(Vector2D moveDirection);

private:
	void SetObject(Object * _object);

	bool inSync;
	const float moveDelay = 0.08f;
	float delayTimer;
	Vector2D oldDirection;
	Vector2D newDirection;

	float movementSpeed;
	PhysicsObject* movableObject;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_MOVE_CONTROLLER