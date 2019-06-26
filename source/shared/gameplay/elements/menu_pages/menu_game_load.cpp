#include "shared/gameplay/elements/menu_pages/menu_game_load.h"
#include "shared/gameplay/elements/animations/fade_animation.h"
#include "shared/gameplay/elements/menu_pages/game_hud.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/elements/game.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* MenuGameLoad::menuFileName = "Data\\Images\\white.png";
const char* MenuGameLoad::loading = "Data\\Images\\loading.png";

MenuGameLoad::MenuGameLoad() : MenuItem()
{
	pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, menuFileName);
	SetText("Loading...");
	size = Camera2D::g_pActiveCamera->virtualScreenSize;
	pFont = Font::g_DefaultLargeFont;
	color = 0xff000000;
	textFormat = DT_CENTER | DT_VCENTER;

	loadingItem = new MenuItem();
	loadingItem->pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, loading);
	loadingItem->size = 40;
	loadingItem->position = Vector2D(0, -pFont->GetFontHeight() - 10);
	this->AddChild(loadingItem);

	AnimationController* fadeController = new FadeAnimation();
	fadeController->SetAnimationTotalTime(0.8f);

	this->AddController(fadeController);
}

void MenuGameLoad::OnEnd()
{
	if (ending)
	{
		Controller* fadeController = this->FindController(ObjectType::FadeAnimation);

		if (fadeController != nullptr && fadeController->IsTypeOf(ObjectType::AnimationController))
		{
			AnimationController* fadeAnimation = static_cast<AnimationController*>(fadeController);
			fadeAnimation->SetAnimationState(AnimationController::AnimationState::BACKWARD);
			fadeAnimation->StartAnimation();
			return;
		}
	}

	auto& game = Game::g_pGame;
	game->GameInit();
	game->gameHUD->Start();

	MenuItem::OnEnd();
}

void MenuGameLoad::Update(float dt)
{
	if (!active)
		return;
	
	loadingItem->rotation -= gradesRot * dt;

	MenuItem::Update(dt);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE