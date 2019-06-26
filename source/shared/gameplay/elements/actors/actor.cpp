#include "shared/gameplay/elements/actors/actor.h"
#include "shared/gameplay/elements/utilities/lifebar.h"
#include "shared/gameplay/elements/animations/move_controller.h"
#include "shared/gameplay/elements/power_ups/power_up_controller.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/constants.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Actor::Actor(Sprite* sprite) : PhysicsObject(sprite)
{
	lifeBar = nullptr;
	shield = nullptr;
	freeze = nullptr;

	spriteAnimation = new SpriteAnimationController<ActorState>();
	this->AddController(spriteAnimation);

	powerUpController = new PowerUpController();
	this->AddController(powerUpController);

	moveController = new MoveController();
	this->AddController(moveController);
}

void Actor::SetMovementSpeed(float setMovementSpeed)
{
	this->movementSpeed = setMovementSpeed;
	moveController->UpdateSpeed(movementSpeed);
}

void Actor::SetRotationSpeed(float setRotationSpeed)
{
	this->rotationSpeed = setRotationSpeed;
}

void Actor::Init()
{
	PhysicsObject::Init();

	// from PhysicsObject
	rigidbody->dragCoef = 1.f;
	rigidbody->dampCoef = 0.1f;

	// from Actor
	movementSpeed = 0.f;
	rotationSpeed = 0.f;
	moveController->Init();

	life = 0;
	invulnerable = false;

	prevActorState = ActorState::INACTIVE;
	currActorState = ActorState::INACTIVE;

	if (lifeBar != nullptr)
	{
		lifeBar->Remove();
		SAFE_DELETE(lifeBar);
	}

	// clears the powerUpEffects
	// and removes the clocks from the actor
	powerUpController->ClearPowerUps();

	// clears the animation running, not the ones loaded
	// the loaded ones are there since the construction of the object
	spriteAnimation->ClearAnimation();
}

void Actor::OnEnd()
{
	powerUpController->ClearPowerUps();

	this->UpdateState(ActorState::DYING);
	spriteAnimation->Play(currActorState);
}

void Actor::Move(Vector2D direction)
{
	moveController->MoveObject(direction);
}

void Actor::Rotate(Vector2D direction, float dt)
{
	const float eps = 1e-3f;
	Vector2D lookDirection = Vector2D(0, 1).Rotate(rotation);

	int iniSign = ((lookDirection.Cross(direction) >= eps) ? 1 : -1);
	float updateAngle = iniSign * rotationSpeed * dt;

	Vector2D nextDirection = lookDirection.Rotate(updateAngle);
	int afterSign = ((nextDirection.Cross(direction) >= eps) ? 1 : -1);

	// check if rotation angle is bigger than needed
	if (iniSign * afterSign <= -eps)
		rotation = atan2f(direction.y, direction.x) * 180.f / ((float) M_PI) - 90.f;
	else
		rotation += updateAngle;
}

void Actor::TakeDamage(int damage)
{
	if (!IsDead() && !invulnerable)
	{
		life -= damage;
		life = life < 0 ? 0 : life;

		if (lifeBar != nullptr)
			lifeBar->UpdateLife(life);

		if (IsDead())
		{
			GameEvent* endGameObjectEvent = new EndGameObjectEvent(GetID());
			Game::g_pGame->gameEventsQueue->PushEvent(endGameObjectEvent);
		}
	}
}

bool Actor::IsDead() const
{
	return life <= 0;
}

int Actor::GetLife()
{
	return life;
}

void Actor::ShowLifeBar()
{
	if (lifeBar == nullptr)
	{
		lifeBar = new LifeBar();
		this->AddChild(lifeBar);
	}

	lifeBar->SetMaxLife(life);
}

void Actor::UpdateLifeBar()
{
	if (lifeBar != nullptr)
		lifeBar->UpdateLife(life);
}

ActorState Actor::GetCurrentState() const
{
	return this->currActorState;
}

ActorState Actor::GetPrevState() const
{
	return this->prevActorState;
}

bool Actor::IsActing() const
{
	return currActorState != ActorState::INACTIVE
		&& currActorState != ActorState::STARTING
		&& currActorState != ActorState::ENDING;
}

void Actor::UpdateState(ActorState actorState)
{
	prevActorState = currActorState;
	currActorState = actorState;
}

void Actor::ApplyPowerUp(PowerUpType powerUpType)
{
	powerUpController->ApplyPowerUp(powerUpType);
}

void Actor::RemovePowerUp(PowerUpType powerUpType)
{
	powerUpController->RemovePowerUp(powerUpType);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE