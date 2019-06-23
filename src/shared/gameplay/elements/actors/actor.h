#ifndef SHARED_GAMEPLAY_ELEMENTS_ACTOR
#define SHARED_GAMEPLAY_ELEMENTS_ACTOR

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/power_ups/power_up_info.h"
#include "shared/gameplay/engine/sprite_animation_controller.h"
#include "shared/gameplay/engine/physics_object.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

enum class ActorState
{
	INACTIVE,
	STARTING,
	WALKING,
	RUNNING,
	ATTACKING,
	DYING,
	ENDING,
};

class LifeBar;
class PowerUpController;
class MoveController;

class Actor : public PhysicsObject
{
public:
	Actor(Sprite* sprite = nullptr);

	ObjectType GetType() const override { return ObjectType::Actor; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Actor) || PhysicsObject::IsTypeOf(type); }

	void OnEnd() override;

	void Init() override;

	// set the moving direction to "direction"
	void Move(Vector2D direction);

	// set the looking direction to "direction"
	// uses delta time for smooth tuning
	void Rotate(Vector2D direction, float dt);

	inline bool IsDead() const;
	//virtual void Die() = 0;

	void TakeDamage(int damage);
	
	int GetLife();

	// shows the lifebar in the game if showLife = true
	void ShowLifeBar();
	void UpdateLifeBar();

	// states related methods
	
	ActorState GetCurrentState() const;
	ActorState GetPrevState() const;
	bool IsActing() const;

	void UpdateState(ActorState actorState);

	// activate powerUp
	void ApplyPowerUp(PowerUpType powerUpType);

	// deactivate powerUp
	void RemovePowerUp(PowerUpType powerUpType);

protected:
	bool invulnerable;
	int life;
	float movementSpeed;
	virtual void SetMovementSpeed(float setMovementSpeed);

	float rotationSpeed;
	virtual void SetRotationSpeed(float setRotationSpeed);

	SpriteAnimationController<ActorState>* spriteAnimation;
	PowerUpController* powerUpController;
	MoveController* moveController;

private:
	LifeBar * lifeBar;
	WorldObject* shield;
	WorldObject* freeze;

	ActorState prevActorState;
	ActorState currActorState;

	friend class PowerUpUtils;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_ACTOR