#ifndef SHARED_GAMEPLAY_ELEMENTS_GAME_HUD
#define SHARED_GAMEPLAY_ELEMENTS_GAME_HUD

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"

// -------------------------------------------------------------------------------------
// This is the HUD menu that appears when you are in the game
// The Game handles showing and hiding this menu page
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class MenuPlayerHUD;

class GameHUD : public MenuItem
{
public:
	GameHUD();

	void OnStart() override;
	void OnEnd() override;

	void ResetInfo();
	MenuPlayerHUD* menuPlayerHUD;

private:
	// returs true if it could animate the element given
	bool AnimateElement(MenuItem* hudElement, bool startingElement);
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_GAME_HUD