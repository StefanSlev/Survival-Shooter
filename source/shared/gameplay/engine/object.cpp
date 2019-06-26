#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/sprite.h"
#include "shared/gameplay/engine/controller.h"
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

int Object::_IDGenerator = 0;
std::unordered_map<int, Object*> Object::objectMap;

void ObjectGlobalValues::Init()
{
	Transform2D::Init();
	colorMask = 0xFFFFFFFF;
}

Object::Object(Sprite *sprite) : UpdateList(), ObjectGlobalValues()
{
	pSprite = sprite;
	spriteTopLeftUV = 0;
	spriteBottomRightUV = 1;

	size = 1;
	color = 0xffffffff;

	active = true;
	visible = true;
	starting = false;
	ending = false;

	pControllers = NULL;

	_ID = _IDGenerator++;
	objectMap.insert(std::pair<int, Object*>(_ID, this));
}

void Object::Init()
{
	ObjectGlobalValues::Init();
	
	global.Init();

	pSprite = nullptr;

	spriteTopLeftUV = 0;
	spriteBottomRightUV = 1;

	size = 1;
	color = 0xffffffff;

	active = true;
	visible = true;
	starting = false;
	ending = false;
}

Object::~Object()
{
	objectMap.erase(_ID);

	while (pControllers)
		delete pControllers;
}

UpdateList* Object::AddChild(UpdateList * child)
{
	if (!child || !child->IsTypeOf(ObjectType::Object) || (IsTypeOf(ObjectType::MenuItem) && !child->IsTypeOf(ObjectType::MenuItem)) || child->IsTypeOf(ObjectType::PhysicsObject))
		return NULL;

	return UpdateList::AddChild(child);
}

Controller* Object::AddController(Controller * controller)
{
	if (!controller) return NULL;

	controller->SetObject(this);
	controller->pBrothers = pControllers;
	pControllers = controller;

	return controller;
}

Controller * Object::FindController(ObjectType type)
{
	Controller *controller = pControllers;
	while (controller) {
		if (controller->IsTypeOf(type))
			return controller;
		controller = (Controller*)controller->pBrothers;
	}

	return nullptr;
}

void Object::Update(float dt)
{
	if (!active)
		return;

	UpdateList *controller = pControllers;
	while (controller) {
		controller->Update(dt);
		controller = controller->pBrothers;
	}

	UpdateList::Update(dt);
}

void Object::Render()
{
	if (!visible)
		return;

	UpdateList *controller = pControllers;
	while (controller) {
		controller->Render();
		controller = controller->pBrothers;
	}

	UpdateList::Render();
}

void Object::DebugRender()
{
	if (!visible)
		return;

	UpdateList *child = pChildren;
	while (child) {
		if (child->IsTypeOf(ObjectType::Object))
			static_cast<Object*>(child)->DebugRender();
		child = child->pBrothers;
	}
}

void Object::UpdateGlobal()
{
	// update global variables as a whole
	global = *(ObjectGlobalValues*)this;
	if (pParent && pParent->IsTypeOf(ObjectType::Object))
	{
		// compute parent position, depending if parent and child are acting in different spaces or not
		Vector2D parentPos;
		if (IsTypeOf(ObjectType::MenuItem) && !pParent->IsTypeOf(ObjectType::MenuItem))
			Camera2D::g_pActiveCamera->WorldSpaceToVirtualScreenSpace(static_cast<Object*>(pParent)->global.position, parentPos);
		else
			parentPos = static_cast<Object*>(pParent)->global.position;

		// compute positions based on parent's rotation, scale and position
		if (static_cast<Object*>(pParent)->global.rotation != 0)
			global.position = position.Rotate(static_cast<Object*>(pParent)->global.rotation);
		global.position *= static_cast<Object*>(pParent)->global.scale;
		global.position += parentPos;

		// add rotation to parent's rotation
		global.rotation = rotation + static_cast<Object*>(pParent)->global.rotation;

		// multiply scale with parent scale
		global.scale = scale * static_cast<Object*>(pParent)->global.scale;

		// multiply color mask with parent color mask
		global.colorMask = sColor(colorMask).Mask(static_cast<Object*>(pParent)->global.colorMask);
	}

	UpdateList::UpdateGlobal();
}

Object * Object::FindByID(int id)
{
	auto result = objectMap.find(id);
	if (result == objectMap.end())
		return nullptr;
	return result->second;
}

void WorldObject::Render()
{
	if (!visible)
		return;

	if (pSprite)
	{
		Vector2D spritePos;
		Vector2D spriteSize;
		float spriteRotation;
		Camera2D::g_pActiveCamera->WorldSpaceToScreenSpace(this, spritePos, spriteSize, spriteRotation);

		// draw the Sprite
		pSprite->Draw(spritePos, spriteSize, spriteRotation, sColor(color).Mask(global.colorMask), spriteTopLeftUV, spriteBottomRightUV);
	}

	Object::Render();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE