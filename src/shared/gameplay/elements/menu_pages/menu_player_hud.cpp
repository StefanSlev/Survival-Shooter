#include "shared/gameplay/elements/menu_pages/menu_player_hud.h"
#include "shared/gameplay/elements/menu_pages/player_hud.h"
#include "shared/gameplay/elements/animations/transform_animation.h"
#include "shared/gameplay/engine/camera.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

MenuPlayerHUD::MenuPlayerHUD() : MenuItem()
{
	size = Camera2D::g_pActiveCamera->virtualScreenSize;
}

void MenuPlayerHUD::ResetInfo()
{
	for (MenuItem* playerHUD : playerHUDs)
	{
		playerHUD->Remove();
		delete playerHUD;
	}

	playerHUDs.clear();
}

void MenuPlayerHUD::AddPlayerHUD(const Player* player)
{
	MenuItem* playerHUD = new PlayerHUD(player);

	if (playerHUDs.size() == 0)
	{
		playerHUD->position = (-1 * size + playerHUD->size) * 0.5f;

		AnimationController* transformController = new TransformAnimation(0, Vector2D(-playerHUD->size.x, 0));
		this->AddController(transformController);
	}
	else
		playerHUD->position = playerHUDs.back()->position + Vector2D(0, playerHUDs.back()->size.y);

	playerHUDs.push_back(playerHUD);
	this->AddChild(playerHUD);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE