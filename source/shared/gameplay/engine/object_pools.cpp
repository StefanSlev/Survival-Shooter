#include "shared/gameplay/engine/object_pools.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

void ObjectPools::CreateObjectPool(ObjectType objectType, ObjectFactory objectFactory)
{
	factories[objectType] = objectFactory;

	// if there is no pool for the specified objectType
	// create a new one, else clear it

	auto it_pool = objectPools.find(objectType);

	if (it_pool == objectPools.end())
		objectPools[objectType] = ObjectPool();
	else
	{
		auto& objectPool = it_pool->second;

		for (Object* object : objectPool)
			delete object;

		objectPool.clear();
	}
	
	this->AllocateObjects(objectType);
}

void ObjectPools::AllocateObjects(ObjectType objectType)
{
	auto it_pool = objectPools.find(objectType);
	auto it_factory = factories.find(objectType);

	if (it_pool != objectPools.end() && it_factory != factories.end())
	{
		auto& objectPool = it_pool->second;
		auto& factory = it_factory->second;

		if (!factory)
			return;

		objectPool.resize(ObjectPools::POOL_SIZE);

		for (unsigned int i = 0; i < objectPool.size(); i++)
			if (!objectPool[i])
				objectPool[i] = factory();
	}
}

void ObjectPools::ClearPools()
{
	for (auto& entry : objectPools)
	{
		ObjectPool& objectPool = entry.second;

		for (Object* object : objectPool)
			delete object;

		objectPool.clear();
	}

	objectPools.clear();
}

ObjectPools::~ObjectPools()
{
	this->ClearPools();
}

Object* ObjectPools::GetObjectFromPool(ObjectType objectType)
{
	auto it = objectPools.find(objectType);

	if (it != objectPools.end())
	{
		auto& objectPool = it->second;

		if (objectPool.size() == 0)
			this->AllocateObjects(objectType);

		Object* object = objectPool.back();

		object->Init();
		objectPool.pop_back();

		return object;
	}

	return nullptr;
}

bool ObjectPools::RecoverObjectInPool(Object* object)
{
	if (object != nullptr)
	{
		auto it = objectPools.find(object->GetType());

		if (it != objectPools.end())
		{
			it->second.push_back(object);
			return true;
		}
	}

	return false;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE