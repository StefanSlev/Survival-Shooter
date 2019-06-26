#include "shared/gameplay/elements/menu_pages/game_hud.h"
#include "shared/gameplay/elements/menu_pages/menu_player_hud.h"
#include "shared/gameplay/elements/animations/animation_controller.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

GameHUD::GameHUD() : MenuItem()
{
	menuPlayerHUD = new MenuPlayerHUD();
	this->AddChild(menuPlayerHUD);
}

void GameHUD::ResetInfo()
{
	menuPlayerHUD->ResetInfo();
}

bool GameHUD::AnimateElement(MenuItem* hudElement, bool startingElement)
{
	Controller* transform = hudElement->FindController(ObjectType::TransformAnimation);

	if (transform != nullptr && transform->IsTypeOf(ObjectType::AnimationController))
	{
		AnimationController* transformAnimation = static_cast<AnimationController*>(transform);
		AnimationController::AnimationState state = (startingElement ? AnimationController::AnimationState::BACKWARD : AnimationController::AnimationState::FORWARD);

		transformAnimation->SetAnimationState(state);
		transformAnimation->StartAnimation();

		return true;
	}

	return false;
}

void GameHUD::OnStart()
{
	if (starting)
	{
		bool animate = AnimateElement(menuPlayerHUD, true);

		if (animate)
			return;
	}

	MenuItem::OnStart();
}

void GameHUD::OnEnd()
{
	if (ending)
	{
		bool animate = AnimateElement(menuPlayerHUD, false);

		if (animate)
			return;
	}

	MenuItem::OnEnd();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE