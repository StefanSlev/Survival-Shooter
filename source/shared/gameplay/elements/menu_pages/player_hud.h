#ifndef SHARED_GAMEPLAY_ELEMENTS_PLAYER_HUD
#define SHARED_GAMEPLAY_ELEMENTS_PLAYER_HUD

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/menu_item.h"

// -------------------------------------------------------------------------------------
// This respresents the hud info of a player
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Sprite;
class Player;

struct PlayerHUDInfo
{
	// player related info
	int playerID;
	long playerColor;
	
	// weapon related info
	Sprite* weaponSprite;
	int numberOfClips;
	int clipSize;
	int bulletsLoaded;
	float reloadTime;
	float remainedReloadTime;
	bool infiniteAmmo;
	bool infiniteClips;
};

class PlayerHUD final : public MenuItem
{
public:
	PlayerHUD(const Player* player);

	void Update(float dt) override;
	void Render() override;
private:
	void UpdateInfo();
	void UpdateItems();
	
	// UI for the PlayerHUD
	MenuItem* weaponItem;
	MenuItem* bulletItem;
	MenuItem* clipsItem;

	PlayerHUDInfo hudInfo;
	const Player* player;

	static const char* bulletFileName;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_PLAYER_HUD