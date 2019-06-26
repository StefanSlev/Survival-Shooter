#ifndef SHARED_GAMEPLAY_ELEMENTS_MENU_GAME_LOAD
#define SHARED_GAMEPLAY_ELEMENTS_MENU_GAME_LOAD

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"

// -------------------------------------------------------------------------------------
// This is the menu page that appears when the game loads
// The Game handles showing and hiding this menu page
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class MenuGameLoad : public MenuItem
{
public:
	MenuGameLoad();
	
	void Update(float dt) override;
	void OnEnd() override;

private:
	static const char* menuFileName;
	static const char* loading;
	MenuItem* loadingItem;
	const static int gradesRot = 360;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_MENU_GAME_LOAD