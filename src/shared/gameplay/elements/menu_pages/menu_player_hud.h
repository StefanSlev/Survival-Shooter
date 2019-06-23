#ifndef SHARED_GAMEPLAY_ELEMENTS_MENU_PLAYER_HUD
#define SHARED_GAMEPLAY_ELEMENTS_MENU_PLAYER_HUD

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"

// -------------------------------------------------------------------------------------
// This is the menu page that shows the playerHUDs
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Player;

class MenuPlayerHUD : public MenuItem
{
public:
	MenuPlayerHUD();

	void AddPlayerHUD(const Player* player);
	void ResetInfo();

private:
	std::vector<MenuItem*> playerHUDs;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_MENU_PLAYER_HUD