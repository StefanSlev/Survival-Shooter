#include "shared/gameplay/elements/utilities/countdown_timer.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

CountdownTimer::CountdownTimer(float totalLifeTime)
{
	this->totalLifeTime = fmaxf(totalLifeTime, 0.f);
	lifeTime = this->totalLifeTime;

	ended = (lifeTime == 0.f) ? true : false;
}

void CountdownTimer::UpdateTime(float dt)
{
	if (ended)
		return;

	if (lifeTime == 0.f)
		ended = true;
	else
	{
		lifeTime = fmaxf(lifeTime - dt, 0.f);
		this->UpdateTimer();
	}
}

void CountdownTimer::AddTime(float time)
{
	totalLifeTime = fmaxf(totalLifeTime + time, 0.f);
	lifeTime = fmaxf(lifeTime + time, 0.f);

	ended = (lifeTime == 0.f) ? true : false;

	this->UpdateTimer();
}

void CountdownTimer::SetTime(float time)
{
	totalLifeTime = fmaxf(time, 0.f);
	lifeTime = totalLifeTime;

	ended = (lifeTime == 0.f) ? true : false;

	this->UpdateTimer();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE