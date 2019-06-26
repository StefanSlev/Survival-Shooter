#include "shared/gameplay/elements/menu_pages/menu_leaderboard.h"
#include "shared/gameplay/elements/menu_pages/leaderboard.h"
#include "shared/gameplay/elements/animations/fade_animation.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* MenuLeaderboard::menuFileName = "Data\\Images\\white.png";

MenuLeaderboard::MenuLeaderboard() : MenuItem()
{
	pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, menuFileName);
	size = Camera2D::g_pActiveCamera->virtualScreenSize;
	color = 0x80808080;
	colorMask = sColor(0, 255, 255, 255).GetValue();

	AnimationController* fadeController = new FadeAnimation();
	this->AddController(fadeController);

	// create the leaderboard
	Vector2D leaderboardSize = Camera2D::g_pActiveCamera->virtualScreenSize;
	leaderboardSize.y = (2.55f * size.y / 3 - 20.f);
	leaderboardSize.x *= (1.f / 3);

	Vector2D leaderboardPosition = Vector2D(0, (this->size.y - size.y) * 0.5f - 30.f);
	
	leaderboard = new Leaderboard(leaderboardSize, leaderboardPosition);
	this->AddChild(leaderboard);
}

void MenuLeaderboard::OnStart()
{
	if (starting)
	{
		leaderboard->Refresh();

		Controller* fadeController = this->FindController(ObjectType::FadeAnimation);

		if (fadeController != nullptr && fadeController->IsTypeOf(ObjectType::AnimationController))
		{
			AnimationController* fadeAnimation = static_cast<AnimationController*>(fadeController);
			fadeAnimation->SetAnimationState(AnimationController::AnimationState::FORWARD);
			fadeAnimation->StartAnimation();
			return;
		}
	}

	MenuItem::OnStart();
}

void MenuLeaderboard::OnEnd()
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

	MenuItem::OnEnd();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE