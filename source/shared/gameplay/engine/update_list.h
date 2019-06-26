#ifndef SHARED_GAMEPLAY_ENGINE_UPDATE_LIST
#define SHARED_GAMEPLAY_ENGINE_UPDATE_LIST

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Base class from which updatable objects are derived
// It also provides a basic hierarchy implementation
// Update is also called down the hierarchy (towards the children)
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class UpdateList
{
public:
	enum class ObjectType : size_t
	{
		Transform2D,
		ObjectGlobalValues,
		UpdateList,
		Object,
		Game,
		Controller,
		SpriteAnimationController,
		AnimationController,
		FadeAnimation,
		TransformAnimation,
		CameraController,
		PowerUpController,
		Camera,
		MenuItem,
		ScrollableList,
		WorldObject,
		PhysicsObject,
		Rigidbody,
		Collider,
		Actor,
		Player,
		Enemy,
		Cyclope,
		Ghoul,
		Kamikaze,
		Spider,
		Undead,
		Spiderling,
		Terrain,
		Weapon,
		Crosshair,
		Bullet,
		GaussBullet,
		Rocket,
		LifeBar,
		Explosion,
		BulletExplosion,
		Splatter,
		RocketExplosion,
		PowerUp,
		NukeExplosion
	};

	UpdateList();
	virtual ~UpdateList();

	virtual UpdateList* AddChild(UpdateList *child);
	virtual void OnAddedAsChild() {}
	virtual void Remove();

	// Override this Update method to implement your own update functionality
	// Don't forget to call the base's class Update method when overriding
	virtual void Update(float dt);

	// Override this Render method to implement your own draw functionality
	// Don't forget to call the base's class Render method when overriding
	virtual void Render();

	// Override this UpdateGlobal method to copy or compute any global values used by the Render step
	// Don't forget to call the base's class UpdateGlobal method when overriding
	virtual void UpdateGlobal();

	virtual ObjectType GetType() const { return ObjectType::UpdateList; }
	virtual bool IsTypeOf(ObjectType type) const { return type == ObjectType::UpdateList; };

	UpdateList *pParent;
	UpdateList *pBrothers;
	UpdateList *pChildren;
};

using ObjectType = UpdateList::ObjectType;
#define GetObjectTypeIndex(objectType) static_cast<std::uint32_t> (objectType)
#define GetObjectType(objectTypeIndex) static_cast<shared::gameplay::ObjectType> (objectTypeIndex)

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_UPDATE_LIST