#include "shared/gameplay/elements/menu_pages/player_hud.h"
#include "shared/gameplay/elements/game_utilities/scoring_system.h"
#include "shared/gameplay/elements/actors/player.h"
#include "shared/gameplay/elements/weapons/weapon.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* PlayerHUD::bulletFileName = "Data\\Images\\white.png";

PlayerHUD::PlayerHUD(const Player* player) : player(player)
{
	SetText("Score: 00000000");
	pFont = Font::g_DefaultLargeFont;
	textFormat = DT_BOTTOM | DT_LEFT;

	Vector2D textSize = pFont->GetTextSize(text, this, textFormat);
	size = Vector2D(textSize.x, textSize.y * 4.f);

	weaponItem = new MenuItem();
	weaponItem->size.y = textSize.y * 2.f;
	weaponItem->position.x = (-size.x + textSize.x) * 0.5f;
	weaponItem->position.y = -size.y * 0.5f + textSize.y * 3.f;

	bulletItem = new MenuItem();
	bulletItem->pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, bulletFileName);
	bulletItem->size.y = textSize.y;
	bulletItem->position.y = -size.y * 0.5f + textSize.y * 1.5f;

	clipsItem = new MenuItem();
	clipsItem->SetText("00");
	clipsItem->pFont = Font::g_DefaultLargeFont;
	clipsItem->textFormat = DT_CENTER | DT_VCENTER;
	clipsItem->size.y = textSize.y;
	clipsItem->position.y = - size.y * 0.5f + textSize.y * 2.5f;

	this->AddChild(weaponItem);
	this->AddChild(bulletItem);
	this->AddChild(clipsItem);

	UpdateInfo();
	UpdateItems();
}

void PlayerHUD::UpdateInfo()
{
	if (player)
	{
		// weapon info
		const Weapon* weapon = player->weapon;

		if (!weapon)
			return;

		const WeaponInfo* weaponInfo = weapon->currentWeapon;

		if (!weaponInfo)
			return;

		hudInfo.weaponSprite = weaponInfo->weaponSprite;
		hudInfo.numberOfClips = weaponInfo->numberOfClips;
		hudInfo.clipSize = weaponInfo->clipSize;
		hudInfo.bulletsLoaded = weaponInfo->bulletsLoaded;
		hudInfo.reloadTime = weaponInfo->reloadTime;
		hudInfo.remainedReloadTime = weapon->reloadTime;
		hudInfo.infiniteAmmo = (weaponInfo->bulletsLoaded == INFINITE_AMMO || weapon->globalInfo.infinite_ammo);
		hudInfo.infiniteClips = (weaponInfo->numberOfClips == INFINITE_CLIPS || weapon->globalInfo.infinite_clips);

		// player info
		hudInfo.playerID = player->GetID();
		hudInfo.playerColor = player->color;
	}
}

void PlayerHUD::UpdateItems()
{
	// update playerHUD
	int score = GetScoringSystem().GetPlayerScore(hudInfo.playerID);

	if (score == ScoringSystem::N_A)
		score = 0;

	std::stringstream hudInfoScore;
	hudInfoScore << "Score: " << std::setw(8) << std::setfill('0') << score;
	sprintf_s(text, MAX_TEXT_LEN, hudInfoScore.str().c_str());

	textColor = hudInfo.playerColor;

	// update weapon
	weaponItem->pSprite = hudInfo.weaponSprite;

	if (hudInfo.weaponSprite)
	{
		float spriteScale = hudInfo.weaponSprite->size.x / hudInfo.weaponSprite->size.y;
		weaponItem->size.x = weaponItem->size.y * spriteScale;
	}

	weaponItem->color = hudInfo.playerColor;

	// update clips
	sprintf_s(clipsItem->text, MAX_TEXT_LEN, "%02d", hudInfo.numberOfClips);
	Vector2D clipsTextSize = clipsItem->pFont->GetTextSize(clipsItem->text, clipsItem, clipsItem->textFormat);

	clipsItem->size.x = clipsTextSize.x;
	clipsItem->position.x = -size.x * 0.5f + clipsItem->size.x * 0.5f;
	clipsItem->textColor = hudInfo.playerColor;

	// update bullets
	bulletItem->size.x = fmaxf(size.x / (2 * hudInfo.clipSize - 1), 0.f);
	bulletItem->color = hudInfo.playerColor;
}

void PlayerHUD::Update(float dt)
{
	if (!active)
		return;

	if (IsAlive() && Game::g_pGame->gameState == Game::GameState::InGame)
	{
		UpdateInfo();
		UpdateItems();
	}

	MenuItem::Update(dt);
}

void PlayerHUD::Render()
{
	if (!visible)
		return;

	// draw the bullets
	for (int i = 1; i <= hudInfo.clipSize; i++)
	{
		// set the position of each bullet
		bulletItem->position.x = -size.x * 0.5f + (4 * i - 3) * 0.5f * bulletItem->size.x;

		// set its color
		if (hudInfo.remainedReloadTime > 0.f)
		{
			// on reload
			int bulletsLoaded = Lerp(0, hudInfo.clipSize, 1.f - hudInfo.remainedReloadTime / hudInfo.reloadTime);
			bulletItem->color = (i <= bulletsLoaded) ? hudInfo.playerColor : sColor(128, 0, 0, 0).GetValue();
		}
		else {

			// when infinite ammo is active
			if (hudInfo.infiniteAmmo)
				bulletItem->color = sColor(255, 204, 0, 0).GetValue();
			else
				// normal case
				bulletItem->color = (i <= hudInfo.bulletsLoaded) ? hudInfo.playerColor : sColor(128, 0, 0, 0).GetValue();
		}

		bulletItem->UpdateGlobal();
		bulletItem->Render();
	}

	// draw the weapon
	weaponItem->UpdateGlobal();
	weaponItem->Render();

	// draw number of clips
	if (!hudInfo.infiniteClips)
	{
		clipsItem->UpdateGlobal();
		clipsItem->Render();
	}

	// draw the text
	if (pText != nullptr || text[0])
		pFont->DrawText(pText != nullptr ? pText : text, this, textFormat, sColor(textColor).Mask(global.colorMask));
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE