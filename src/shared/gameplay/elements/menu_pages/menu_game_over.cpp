//// ©2018 Ubisoft Bucharest
//// This file is property of Ubisoft Bucharest, you are not allowed to distribute it.
//
//#include "stdafx.h"
//#include "Game/MenuPages/MenuGameOver.h"
//#include "Game/Animations/FadeAnimation.h"
//
//const char* MenuGameOver::menuFileName = "Data\\Images\\white.png";
//
//MenuGameOver::MenuGameOver() : MenuItem()
//{
//	pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, menuFileName);
//	SetText("Game Over !");
//	size = Camera2D::g_pActiveCamera->virtualScreenSize;
//	pFont = Font::g_DefaultLargeFont;
//
//	color = 0xff000000;
//	colorMask = sColor(0, 255, 255, 255).GetValue();
//	
//	textFormat = DT_CENTER | DT_VCENTER;
//
//	AnimationController* fadeController = new FadeAnimation();
//	this->AddController(fadeController);
//}
//
//void MenuGameOver::OnStart()
//{
//	if (starting)
//	{	
//		Game::g_pGame->gameHUD->End();
//		Game::g_pGame->gameState = Game::GameState::GameOver;
//
//		Controller* fadeController = this->FindController(ObjectType::FadeAnimation);
//
//		if (fadeController != nullptr && fadeController->IsTypeOf(ObjectType::AnimationController))
//		{
//			AnimationController* fadeAnimation = static_cast<AnimationController*>(fadeController);
//			fadeAnimation->SetAnimationState(AnimationController::AnimationState::FORWARD);
//			fadeAnimation->SetAnimationTotalTime(4.f);
//			fadeAnimation->StartAnimation();
//			return;
//		}
//	}
//
//	MenuItem::OnStart();
//
//	// immediately make it disappear
//	MenuItem::End();
//}
//
//void MenuGameOver::OnEnd()
//{
//	if (ending)
//	{
//		Game::g_pGame->GameCleanup();
//		Controller* fadeController = this->FindController(ObjectType::FadeAnimation);
//
//		if (fadeController != nullptr && fadeController->IsTypeOf(ObjectType::AnimationController))
//		{
//			AnimationController* fadeAnimation = static_cast<AnimationController*>(fadeController);
//			fadeAnimation->SetAnimationState(AnimationController::AnimationState::BACKWARD);
//			fadeAnimation->SetAnimationTotalTime(0.4f);
//			fadeAnimation->StartAnimation();
//			return;
//		}
//	}
//	
//	Game::g_pGame->menuLeaderboard->Start();
//	MenuItem::OnEnd();
//}