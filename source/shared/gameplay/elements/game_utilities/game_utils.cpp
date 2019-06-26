#include "shared/gameplay/elements/game_utilities/game_utils.h"
#include "shared/gameplay/engine/object_pools.h"
#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const GameUtils::ObjectPredicate GameUtils::CheckAlive = [](Object* object) {return object && object->IsAlive(); };
const GameUtils::ObjectPredicate GameUtils::CheckNotAlive = [](Object* object) {return !object || !object->IsAlive(); };
const GameUtils::ObjectPredicate GameUtils::CheckActive = [](Object* object) {return object && object->active; };
const GameUtils::ObjectPredicate GameUtils::CheckNotActive = [](Object* object) {return !object || !object->active; };

void GameUtils::GetAliveObjects(const std::vector<Object*>& objects, std::vector<Object*>& aliveObjects)
{
	aliveObjects.reserve(objects.size());

	for (unsigned int i = 0; i < objects.size(); i++)
		if (CheckAlive(objects[i]))
			aliveObjects.push_back(objects[i]);
}

void GameUtils::CheckCollision(Object* firstObject, Object* secondObject,
	float firstRadiusProc, float secondRadiusProc)
{
	if (!firstObject->IsTypeOf(ObjectType::PhysicsObject) ||
		!secondObject->IsTypeOf(ObjectType::PhysicsObject))
			return;

	PhysicsObject* first = static_cast<PhysicsObject*> (firstObject);
	PhysicsObject* second = static_cast<PhysicsObject*> (secondObject);

	// save each radius
	float firstRadius = first->collider->radius;
	float secondRadius = second->collider->radius;

	// update radius with radiusProc
	first->collider->radius *= firstRadiusProc;
	second->collider->radius *= secondRadiusProc;

	// check collision
	first->CheckCollision(*second);

	// restore each radius
	first->collider->radius = firstRadius;
	second->collider->radius = secondRadius;
}

void GameUtils::CheckCollision(const std::vector<Object*>& objects, float radiusProc)
{
	if (!objects.size())
		return;

	std::vector<Object*> aliveObjects;

	GetAliveObjects(objects, aliveObjects);

	if (!aliveObjects.size())
		return;

	for (unsigned int i = 0; i < aliveObjects.size() - 1; i++)
		for (unsigned int j = i + 1; j < aliveObjects.size(); j++)
			CheckCollision(aliveObjects[i], aliveObjects[j], radiusProc, radiusProc);
}

void GameUtils::CheckCollision(const std::vector<Object*>& firstSet, const std::vector<Object*>& secondSet,
	float firstRadiusProc, float secondRadiusProc)
{
	if (!firstSet.size() || !secondSet.size())
		return;

	std::vector<Object*> firstAliveSet;
	std::vector<Object*> secondAliveSet;

	GetAliveObjects(firstSet, firstAliveSet);
	GetAliveObjects(secondSet, secondAliveSet);

	if (!firstAliveSet.size() || !secondAliveSet.size())
		return;

	for (unsigned int i = 0; i < firstAliveSet.size(); i++)
		for (unsigned int j = 0; j < secondAliveSet.size(); j++)
			CheckCollision(firstAliveSet[i], secondAliveSet[j], firstRadiusProc, secondRadiusProc);
}

void GameUtils::UpdateGameObjects(std::vector<Object*>& gameObjects, std::function<void(Object*)> EndGameObject, float deltaTime)
{
	unsigned int nrObjects = static_cast<unsigned int> (gameObjects.size());

	if (!nrObjects)
		return;

	int firstIndex = 0;
	int lastIndex = nrObjects - 1;

	while (firstIndex < lastIndex)
	{
		while (CheckActive(gameObjects[firstIndex]) && firstIndex < (int)nrObjects - 1)
			firstIndex++;

		while (!CheckActive(gameObjects[lastIndex]) && lastIndex > 0)
			lastIndex--;

		if (firstIndex <= lastIndex)
		{
			std::swap(gameObjects[firstIndex], gameObjects[lastIndex]);
			firstIndex++;
			lastIndex--;
		}
	}

	unsigned int inactiveObjects = 0;

	// update the objects

	for (unsigned int index = 0; index < nrObjects; index++)
	{
		if (CheckActive(gameObjects[index]))
			gameObjects[index]->Update(deltaTime);
		else
		{
			if (EndGameObject)
				EndGameObject(gameObjects[index]);

			++inactiveObjects;
		}
	}

	if (inactiveObjects)
		gameObjects.resize(nrObjects - inactiveObjects);
}

void GameUtils::UpdatePoolObjects(std::vector<Object*>& poolObjects, float deltaTime)
{
	UpdateGameObjects(poolObjects, [](Object* object) {

		ObjectPools::GetInstance().RecoverObjectInPool(object);
	}, deltaTime);
}

void GameUtils::UpdatePhysics(const std::vector<Object*>& gameObjects, float deltaTime)
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
		if (gameObjects[i] && gameObjects[i]->IsTypeOf(ObjectType::PhysicsObject))
		{
			static_cast<PhysicsObject*>(gameObjects[i])->UpdatePhysics(deltaTime);
		}
}

void GameUtils::UpdateGlobal(const std::vector<Object*>& objects)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		if (objects[i])
			objects[i]->UpdateGlobal();
}

void GameUtils::Render(const std::vector<Object*>& objects, ObjectsState objectsState)
{
	ObjectPredicate checkObject = nullptr;

	switch (objectsState)
	{
	case ObjectsState::Alive:
		checkObject = CheckAlive;
		break;
	case ObjectsState::NotAlive:
		checkObject = CheckNotAlive;
		break;
	case ObjectsState::All:
		checkObject = [](Object* object) {return object; };
		break;
	default:
		break;
	}

	if (!checkObject)
		return;

	for (unsigned int i = 0; i < objects.size(); i++)
		if (checkObject(objects[i]))
			objects[i]->Render();
}

void GameUtils::DebugRender(const std::vector<Object*>& gameObjects, ObjectsState objectsState)
{
	ObjectPredicate checkObject = nullptr;

	switch (objectsState)
	{
	case ObjectsState::Alive:
		checkObject = CheckAlive;
		break;
	case ObjectsState::NotAlive:
		checkObject = CheckNotAlive;
		break;
	case ObjectsState::All:
		checkObject = [](Object* object) {return object; };
		break;
	default:
		break;
	}

	if (!checkObject)
		return;

	for (unsigned int i = 0; i < gameObjects.size(); i++)
		if (checkObject(gameObjects[i]))
			gameObjects[i]->DebugRender();
}

void GameUtils::DestroyPoolObjects(std::vector<Object*>& poolObjects)
{
	for (unsigned int i = 0; i < poolObjects.size(); i++)
		if (poolObjects[i])
		{
			bool recovered = ObjectPools::GetInstance().RecoverObjectInPool(poolObjects[i]);

			if (!recovered)
				delete poolObjects[i];
		}

	poolObjects.clear();
}

void GameUtils::DestroyObjects(std::vector<Object*>& objects)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		if (objects[i])
			delete objects[i];

	objects.clear();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE