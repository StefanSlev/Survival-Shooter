#ifndef SHARED_GAMEPLAY_ELEMENTS_PLAYER
#define SHARED_GAMEPLAY_ELEMENTS_PLAYER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/actors/actor.h"

// -------------------------------------------------------------------------------------
// Class that defines the player.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Weapon;
struct InputDevice;

class Player : public Actor
{
public:
	Player();
	
	ObjectType GetType() const override { return ObjectType::Player; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Player) || Actor::IsTypeOf(type); }

	void Update(float dt) override;
	void UpdatePhysics(float dt) override;

	void Init() override;
	void OnEnd() override;

	void SetInputDevice(const InputDevice* setInputDevice);

	Weapon* weapon;
	const InputDevice* inputDevice;

private:
	// adjust the move of the player
	void HandleWalk(float dt);
	void HandleWeapons();

	static const char* walkAnim;
	static const char* deathAnim;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_PLAYER