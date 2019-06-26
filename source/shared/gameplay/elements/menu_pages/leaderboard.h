#ifndef SHARED_GAMEPLAY_ELEMENTS_LEADERBOARD
#define SHARED_GAMEPLAY_ELEMENTS_LEADERBOARD

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"
#include "shared/gameplay/engine/scrollable_list.h"

// -------------------------------------------------------------------------------------
// This is the leaderboard that appears on game start
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Leaderboard : public MenuItem
{
public:
	Leaderboard(Vector2D size, Vector2D position);
	void Refresh();

private:
	static const char* menuFileName;
	ScrollableList* scrollableList;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_LEADERBOARD