#include "shared/gameplay/elements/utilities/countdown_clock.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/sprite.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* CountdownClock::halfCirclePath = "Data\\Images\\half_circle.png";

CountdownClock::CountdownClock() : CountdownTimer(), WorldObject(nullptr) 
{
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	Sprite* halfCircleSprite = resourceManager.GetSprite(ResourceType::IMAGE, halfCirclePath);

	leftSide[BACKGROUND] = new WorldObject(halfCircleSprite);
	this->AddChild(leftSide[BACKGROUND]);

	leftSide[FOREGROUND] = new WorldObject(halfCircleSprite);
	this->AddChild(leftSide[FOREGROUND]);

	rightSide[BACKGROUND] = new WorldObject(halfCircleSprite);
	this->AddChild(rightSide[BACKGROUND]);

	rightSide[FOREGROUND] = new WorldObject(halfCircleSprite);
	this->AddChild(rightSide[FOREGROUND]);

	clockIcon = new WorldObject();
	this->AddChild(clockIcon);

	// Initiliaze properties
	leftSide[BACKGROUND]->color = rightSide[BACKGROUND]->color = backgroundColor;
	leftSide[FOREGROUND]->color = rightSide[FOREGROUND]->color = foregroundColor;

	for (WorldObject* halfCircle : rightSide)
		halfCircle->rotation = -180.f;
	
	this->SetSize(0.9f);
	this->SetTime(0.f);
}

void CountdownClock::SetSize(float setSize)
{
	setSize = fmaxf(setSize, 0.f);

	clockIcon->size = 0.85f * setSize;

	for (WorldObject* halfCircle : leftSide)
		halfCircle->size = setSize;

	for (WorldObject* halfCircle : rightSide)
		halfCircle->size = setSize;
}

void CountdownClock::LoadClockIcon(Sprite* clockIconSprite)
{
	clockIcon->pSprite = clockIconSprite;
}

void CountdownClock::Render()
{
	if (!visible)
		return;

	clockIcon->Render();

	if (lifeTime * 2.f > totalLifeTime)
		rightSide[BACKGROUND]->Render();

	rightSide[FOREGROUND]->Render();
	leftSide[BACKGROUND]->Render();
	leftSide[FOREGROUND]->Render();

	if (lifeTime * 2.f <= totalLifeTime)
		rightSide[BACKGROUND]->Render();
}

void CountdownClock::UpdateTimer()
{
	if (totalLifeTime == 0.f)
		return;

	float doubleProc = lifeTime * 2.f / totalLifeTime;

	if (lifeTime * 2.f <= totalLifeTime)
	{
		leftSide[FOREGROUND]->rotation = Lerp(-180.f, 0.f, doubleProc);
		rightSide[FOREGROUND]->rotation = -360.f;
	}
	else
	{
		leftSide[FOREGROUND]->rotation = 0.f;
		rightSide[FOREGROUND]->rotation = Lerp(-180.f, -360.f, 2.f - doubleProc);
	}
}

void CountdownClock::Update(float dt)
{
	if (!active)
		return;

	this->UpdateTime(dt);

	WorldObject::Update(dt);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE