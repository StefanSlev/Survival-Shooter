#include "shared/gameplay/elements/actors/enemy.h"
#include "shared/gameplay/elements/animations/fade_animation.h"
#include "shared/gameplay/elements/animations/move_controller.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/terrain.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/constants.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Enemy::Enemy(Sprite* sprite) : Actor(sprite)
{	
	target = nullptr;

	// ending animation
	AnimationController* fadeController = new FadeAnimation();
	this->AddController(fadeController);
}

void Enemy::SetRotationSpeed(float setRotationSpeed)
{
	Actor::SetRotationSpeed(setRotationSpeed);

	this->iniRotationSpeed = setRotationSpeed;
}

void Enemy::SetMovementSpeed(float setMovementSpeed)
{
	Actor::SetMovementSpeed(setMovementSpeed);

	this->iniMovementSpeed = setMovementSpeed;
}

void Enemy::Init()
{
	Actor::Init();

	// from Actor
	life = 10;
	this->UpdateState(ActorState::STARTING);

	// from Enemy
	this->SetMovementSpeed(1.f);
	this->SetRotationSpeed(0.f);

	score = 100;
	damage = 1;
	hitRate = 0.66f;
	attackRange = 0.1f;
	canAttack = true;

	changedTarget = false;
	target = nullptr;
	waitForAttack = 0.f;

	/** calculate spawing position **/

	Terrain* terrain = Game::g_pGame->terrain;
	const Vector2D spawnBox = terrain->playingAreaFence->size / 2 + terrain->marginArea;

	int top_bottom = rand() % 2;
	int chooseSide = rand() % 2;

	if (top_bottom) //go top or bottom
	{
		position.x = Rand(-spawnBox.x, spawnBox.x);
		position.y = chooseSide * - spawnBox.y + (1 - chooseSide) * spawnBox.y;
	}
	else { // go left or right

		position.x = chooseSide * - spawnBox.x + (1 - chooseSide) * spawnBox.x;
		position.y = Rand(-spawnBox.y, spawnBox.y);
	}

	/** calculate velocity **/
	rigidbody->velocity = Vector2D(0) - position;
	rigidbody->velocity.SafeNormalize();
	rigidbody->velocity *= movementSpeed;

	/** calculate rotation **/
	rotation = atan2f(rigidbody->velocity.y, rigidbody->velocity.x) * 180.f / M_PI - 90.f;

	// ini fade animation
}

void Enemy::Update(float dt)
{
	if (IsAlive())
	{
		if (IsActing())
		{
			this->HandleAttack();
			this->HandleWalk(dt);
			waitForAttack = fmaxf(0.f, waitForAttack - dt);
		}
		else
		{
			// if is not acting, it stays still; useful
			// when enemy is frozen or in dying animation
			rigidbody->velocity = 0.f;
		}
	}

	Actor::Update(dt);
}

void Enemy::HandleAttack()
{
	bool inPosition = false;

	if (target)
	{
		float sumRadius = collider->radius + target->collider->radius;
		float finalAttackDist = (sumRadius + attackRange);

		Vector2D toPlayerDirection = target->position - position;
		Vector2D lookDirection = Vector2D(0, 1).Rotate(rotation);

		float dot = lookDirection.Dot(toPlayerDirection);
		float det = lookDirection.Cross(toPlayerDirection);
		float toPlayerAngle = atan2f(det, dot) * 180.f / M_PI;

		inPosition = (toPlayerDirection.LengthSq() <= finalAttackDist * finalAttackDist
			&& fabsf(toPlayerAngle) <= attackAngle);
	}
	
	if (inPosition)
	{
		this->UpdateState(ActorState::ATTACKING);
		rigidbody->velocity = 0.f;

		if (canAttack && waitForAttack == 0.f && !spriteAnimation->IsPlaying(ActorState::ATTACKING))
		{
			spriteAnimation->Play(GetCurrentState());

			// generate damage event
			GameEvent* gameEvent = new DamageActorEvent(GetID(), target->GetID(), damage);
			Game::g_pGame->gameEventsQueue->PushEvent(gameEvent);

			waitForAttack = 1.f / hitRate;
		}
	}
	else if (GetCurrentState() == ActorState::ATTACKING)
	{
		if (canAttack)
		{
			if (!spriteAnimation->IsPlaying(GetCurrentState()))
			{
				this->UpdateState(ActorState::WALKING);
				Actor::Move(Vector2D(0, 1).Rotate(rotation));
				spriteAnimation->Play(GetCurrentState());
			}
			else rigidbody->velocity = 0.f;

		}
		else 
		{
			this->UpdateState(ActorState::WALKING);
			Actor::Move(Vector2D(0, 1).Rotate(rotation));
		}
	}
}

void Enemy::HandleWalk(float dt)
{
	if (GetCurrentState() != ActorState::WALKING)
		return;

	Vector2D lookDirection = Vector2D(0, 1).Rotate(rotation);

	if (target == nullptr)
	{
		Actor::Move(lookDirection);
		return;
	}

	const float eps = 1e-3f;

	Vector2D finalDirection = target->position - position;

	Actor::Rotate(finalDirection, dt);

	Vector2D newLookDirection = Vector2D(0, 1).Rotate(rotation);

	int iniSign = ((lookDirection.Cross(finalDirection) >= eps) ? 1 : -1);
	int afterSign = ((newLookDirection.Cross(finalDirection) >= eps) ? 1 : -1);

	// check if rotation angle is bigger than needed
	if (iniSign * afterSign <= -eps)
		Actor::Move(finalDirection);
	else
		Actor::Move(newLookDirection);

	this->AdjustCircularMove(iniSign, dt);
}

void Enemy::AdjustCircularMove(int lastStepSign, float dt)
{
	Vector2D lookDirection = Vector2D(0, 1).Rotate(rotation);
	Vector2D finalDirection = target->position - position;

	// the circle it makes my rotating and moving continuously
	std::pair<Vector2D, float> moveCircle;

	// radius of the circle
	moveCircle.second = (iniMovementSpeed * 180.f) / (iniRotationSpeed * M_PI);

	// tangential speed is perpendicular to the radius of the circle
	Vector2D toCenter = lookDirection.Rotate(lastStepSign * 90.f);

	// the center of the circle made in the circular movement
	moveCircle.first = position + toCenter * moveCircle.second;

	// distance from the circle to the player's position
	float centerToPlayer = Dist(moveCircle.first, target->position);

	float radiusSum = collider->radius + target->collider->radius;
	
	// calculate how close they are to collide
	float enemyToPlayer = fmaxf(Dist(position, target->position) - radiusSum, 0.f);

	float enterCircleProc = fminf(centerToPlayer / moveCircle.second, 1.f);
	float closeToEnemyProc = fminf(enemyToPlayer * 2.f / moveCircle.second, 1.f);

	// calculate the angle between the look direction and the direction to the player 
	float dot = lookDirection.Dot(finalDirection);
	float det = lookDirection.Cross(finalDirection);
	float instantUpdateAngle = atan2f(det, dot) * 180.f / M_PI;

	// update rotation and movement speed so that the enemy can slow down and turn faster when the player
	// gets near; this way the enemy won't pass by the followed player for great movementSpeed or little rotationSpeed
	this->rotationSpeed = Lerp(fabsf(instantUpdateAngle) / dt, iniRotationSpeed, enterCircleProc);
	this->movementSpeed = Lerp(iniMovementSpeed * 0.5f, iniMovementSpeed, closeToEnemyProc);
}

void Enemy::OnEnd()
{
	ActorState currentState = this->GetCurrentState();

	if (currentState != ActorState::DYING && currentState != ActorState::ENDING)
		Actor::OnEnd();
	else if (currentState != ActorState::ENDING)
	{
		this->UpdateState(ActorState::ENDING);

		Controller* fadeController = this->FindController(ObjectType::FadeAnimation);

		if (fadeController != nullptr && fadeController->IsTypeOf(ObjectType::AnimationController))
		{
			AnimationController* fadeAnimation = static_cast<AnimationController*>(fadeController);
			fadeAnimation->SetAnimationState(AnimationController::AnimationState::BACKWARD);
			fadeAnimation->StartAnimation();
		}
	}
	else
		PhysicsObject::OnEnd();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE