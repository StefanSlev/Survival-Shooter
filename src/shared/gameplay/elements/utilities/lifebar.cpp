#include "shared/gameplay/elements/utilities/lifebar.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const long LifeBar::greenColor = sColor(255, 0, 255, 0).GetValue();
const long LifeBar::yellowColor = sColor(255, 255, 255, 0).GetValue();
const long LifeBar::redColor = sColor(255, 255, 0, 0).GetValue();
const char* LifeBar::lifeBarFileName = "Data\\Images\\white.png";

LifeBar::LifeBar() : WorldObject()
{
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	Sprite* lifeBarSprite = resourceManager.GetSprite(ResourceType::IMAGE, lifeBarFileName);

	// remained life
	this->fullBar = new WorldObject(lifeBarSprite);
	// lost life
	this->emptyBar = new WorldObject(lifeBarSprite);
	
	this->AddChild(fullBar);
	this->AddChild(emptyBar);

	Init();
}

void LifeBar::OnAddedAsChild()
{
	if (pParent->IsTypeOf(ObjectType::PhysicsObject))
	{
		PhysicsObject* parent = static_cast<PhysicsObject*>(pParent);
		
		position = Vector2D(0, -parent->collider->radius);
		size = Vector2D(parent->collider->radius + 0.3f, 0.2f);
	}
}

void LifeBar::Init() {

	maxLife = 0;
	lifeProc = 0.f;
	size = { 1.3f, 0.2f };

	// behind the actor
	position = { 0, -1 };

	// transparency effect
	colorMask = sColor(128, 255, 255, 255).GetValue();

	fullBar->size.y = emptyBar->size.y = size.y;
	emptyBar->color = sColor(255, 119, 136, 153).GetValue();

	this->Refresh();
}

void LifeBar::SetMaxLife(int setMaxLife)
{
	this->maxLife = setMaxLife;
	lifeProc = 1.f;
	this->UpdateLife(setMaxLife);
}

void LifeBar::Refresh()
{
	const float eps = 1e-6f;

	fullBar->size.x = lifeProc * this->size.x;
	emptyBar->size.x = (1 - lifeProc) * this->size.x;

	fullBar->position.x = Lerp(-size.x * 0.5f, 0.f, lifeProc);
	emptyBar->position.x = Lerp(0.f, size.x * 0.5f, lifeProc);

	if (lifeProc - 0.5f > -eps)
		fullBar->color = LerpColor(yellowColor, greenColor, 2.f * lifeProc - 1.f);
	else
		fullBar->color = LerpColor(redColor, yellowColor, 2.f * lifeProc);
}

void LifeBar::UpdateLife(int life)
{
	this->lifeProc = (maxLife <= 0 ? 0.f : life * 1.0f / maxLife);
	this->Refresh();	
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE