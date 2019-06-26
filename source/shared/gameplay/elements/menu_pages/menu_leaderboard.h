#ifndef SHARED_GAMEPLAY_ELEMENTS_MENU_LEADERBOARD
#define SHARED_GAMEPLAY_ELEMENTS_MENU_LEADERBOARD

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"

// -------------------------------------------------------------------------------------
// This is the menu page that appears when the game starts
// The Game handles showing and hiding this menu page
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Leaderboard;

class MenuLeaderboard : public MenuItem
{
public:
	MenuLeaderboard();

	void OnStart() override;
	void OnEnd() override;
	
	Leaderboard* leaderboard;
private:
	static const char* menuFileName;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_MENU_LEADERBOARD