#ifndef SHARED_GAMEPLAY_ELEMENTS_COUNTDOWN_CLOCK
#define SHARED_GAMEPLAY_ELEMENTS_COUNTDOWN_CLOCK

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/utilities/countdown_timer.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/utils.h"

// -------------------------------------------------------------------------------------
// Class that defines a circular countdown clock
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class CountdownClock : public CountdownTimer, public WorldObject
{
public:
	CountdownClock();

	void Update(float dt) override;
	void UpdateTimer() override;

	void Render() override;

	void LoadClockIcon(Sprite* clockIconSprite = nullptr);
	void SetSize(float setSize);

private:
	WorldObject* leftSide[2];
	WorldObject* rightSide[2];
	WorldObject* clockIcon;

	enum {

		BACKGROUND,
		FOREGROUND
	};

	const long backgroundColor = sColor(255, 128, 128, 128).GetValue();
	const long foregroundColor = -1;
	static const char* halfCirclePath;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_COUNTDOWN_CLOCK