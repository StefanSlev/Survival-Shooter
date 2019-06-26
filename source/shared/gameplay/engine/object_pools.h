#ifndef SHARED_GAMEPLAY_ENGINE_OBJECT_POOLS
#define SHARED_GAMEPLAY_ENGINE_OBJECT_POOLS

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"

// -------------------------------------------------------------------------------------
// Object Pools
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class ObjectPools
{
public:
	using ObjectFactory = std::function<Object*(void)>;
	using ObjectPool = std::vector<Object*>;

	const static int POOL_SIZE = 10;

	static ObjectPools& GetInstance()
	{
		static ObjectPools instance;
		return instance;
	}
	~ObjectPools();

	void CreateObjectPool(ObjectType objectType, ObjectFactory objectFactory);
	
	Object* GetObjectFromPool(ObjectType objectType);
	
	// returns true if the operation was successful
	bool RecoverObjectInPool(Object* object);

	void ClearPools();
private:
	std::unordered_map<ObjectType, ObjectFactory> factories;
	std::unordered_map<ObjectType, ObjectPool> objectPools;

	void AllocateObjects(ObjectType objectType);

	ObjectPools() {}
	ObjectPools(const ObjectPools&) = delete;
	void operator= (const ObjectPools&) = delete;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#define CreateObjectFactory(_ObjectType) [](void) -> Object* {return new _ObjectType(); }

#endif // SHARED_GAMEPLAY_ENGINE_OBJECT_POOLS