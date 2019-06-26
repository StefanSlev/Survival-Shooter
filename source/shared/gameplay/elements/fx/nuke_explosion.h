#ifndef SHARED_GAMEPLAY_ELEMENTS_NUKE_EXPLOSION
#define SHARED_GAMEPLAY_ELEMENTS_NUKE_EXPLOSION

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"

// -------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class NukeExplosion : public MenuItem
{
public:
	NukeExplosion();

	ObjectType GetType() const override { return ObjectType::NukeExplosion; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::NukeExplosion) || MenuItem::IsTypeOf(type); }

	void Init(int initIdGenerator) {  this->idGenerator = initIdGenerator;}

	void Update(float dt) override;
	void OnEnd() override;

private:
	int idGenerator;
	float lifeTime;
	const float totalLifeTime = 1.5f;
	const float growthFactor = 2.f / totalLifeTime;

	static const char* nukeFileName;
	static const int nukeDamage = 1000000;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_NUKE_EXPLOSION