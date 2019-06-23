#ifndef SHARED_GAMEPLAY_ELEMENTS_COUNTDOWN_TIMER
#define SHARED_GAMEPLAY_ELEMENTS_COUNTDOWN_TIMER

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Class that defines a CountdownTimer
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class CountdownTimer
{
public:
	CountdownTimer(float totalLifeTime = 0.f);

	void UpdateTime(float dt);
	virtual void UpdateTimer() = 0;
	
	void AddTime(float time);
	void SetTime(float time);

	bool HasEnded() const { return ended; }

protected:
	float lifeTime;
	float totalLifeTime;
	bool ended;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_COUNTDOWN_TIMER