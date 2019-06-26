#ifndef SHARED_GAMEPLAY_ELEMENTS_LIFEBAR
#define SHARED_GAMEPLAY_ELEMENTS_LIFEBAR

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"

// -------------------------------------------------------------------------------------
// Class that defines the life bar of the player.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class LifeBar : public WorldObject
{
public:
	LifeBar();

	ObjectType GetType() const override { return ObjectType::LifeBar; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::LifeBar) || WorldObject::IsTypeOf(type); }

	void Init();
	
	void UpdateLife(int life);
	void SetMaxLife(int setMaxLife);

private:
	void OnAddedAsChild() override;

	WorldObject* fullBar;
	WorldObject* emptyBar;
	float lifeProc;
	int maxLife;

	void Refresh();

	static const long greenColor;
	static const long yellowColor;
	static const long redColor;
	static const char* lifeBarFileName;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_LIFEBAR