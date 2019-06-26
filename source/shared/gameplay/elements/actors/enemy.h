#ifndef SHARED_GAMEPLAY_ELEMENTS_ENEMY
#define SHARED_GAMEPLAY_ELEMENTS_ENEMY

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/actors/actor.h"

// -------------------------------------------------------------------------------------
// Class that defines an enemy.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Enemy : public Actor
{
public:
	Enemy(Sprite* sprite = nullptr);
	void Init() override;

	ObjectType GetType() const override { return ObjectType::Enemy; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Enemy) || Actor::IsTypeOf(type); }

	void Update(float dt) override;
	void OnEnd() override;

	int GetScore() const { return score; }

	void SetTarget(Actor* setTarget) { this->target = setTarget; }
	Actor* GetTarget() const { return target; }

	bool ChangedTarget() const { return changedTarget; }

protected:
	int score;
	int damage;
	float hitRate;
	float attackRange;
	bool canAttack;

	// sets both the current and the initial rotationSpeed
	void SetRotationSpeed(float setRotationSpeed) override;
	// sets both the current and the initial movementSpeed
	void SetMovementSpeed(float setMovementSpeed) override;

	virtual void HandleAttack();

	Actor* target;
	bool changedTarget;

private:
	// timer for attack
	float waitForAttack;

	// the angle from its looking direction
	// the enemy can detect a player and attack him
	const float attackAngle = 45.f;

	// adjust the move of the enemy in order to
	// follow its target
	void HandleWalk(float dt);
	void AdjustCircularMove(int lastStepSign, float dt);

	float iniRotationSpeed;
	float iniMovementSpeed;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_ENEMY