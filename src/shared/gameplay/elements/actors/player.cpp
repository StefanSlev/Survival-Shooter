#include "shared/gameplay/elements/actors/player.h"
#include "shared/gameplay/elements/animations/fade_animation.h"
#include "shared/gameplay/elements/game_utilities/game_input.h"
#include "shared/gameplay/elements/weapons/weapon.h"
#include "shared/gameplay/elements/terrain.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* Player::walkAnim = "Data\\Anims\\anim_player_walk.png";
const char* Player::deathAnim = "Data\\Anims\\anim_player_death.png";

Player::Player() : Actor(nullptr)
{
	// ending animation
	AnimationController* fadeController = new FadeAnimation();
	this->AddController(fadeController);

	this->inputDevice = nullptr;

	this->weapon = new Weapon();
	this->AddController(weapon);

	// load animations

	ResourceManager& resourceManager = ResourceManager::GetInstance();

	SpriteAnimationInfo<ActorState>* walkAnimation = new SpriteAnimationInfo<ActorState>();
	walkAnimation->animationType = ActorState::WALKING;
	walkAnimation->animationSprite = resourceManager.GetSprite(ResourceType::ANIMATION, walkAnim);
	walkAnimation->frameSize = 256;
	walkAnimation->startIndex = 0;
	walkAnimation->animationLength = 16;
	walkAnimation->fps = 60.f;
	walkAnimation->looping = true;

	SpriteAnimationInfo<ActorState>* deathAnimation = new SpriteAnimationInfo<ActorState>();
	deathAnimation->animationType = ActorState::DYING;
	deathAnimation->animationSprite = resourceManager.GetSprite(ResourceType::ANIMATION, deathAnim);
	deathAnimation->frameSize = 256;
	deathAnimation->startIndex = 0;
	deathAnimation->animationLength = 16;
	deathAnimation->fps = 20.f;
	deathAnimation->looping = false;
	deathAnimation->OnEnd = &Object::OnEnd;

	spriteAnimation->Load(walkAnimation);
	spriteAnimation->Load(deathAnimation);
}

void Player::Init()
{
	Actor::Init();

	// from Object
	size = 2.f;
	collider->ComputeDefaultRadius();
	collider->radius *= 0.75f;

	// from PhysicsObject
	rigidbody->mass = 1000;

	// from Actor
	this->SetMovementSpeed(5.f);
	this->SetRotationSpeed(720.f);
	life = 100;

	Actor::ShowLifeBar();
	
	// initialize the walking state
	this->UpdateState(ActorState::WALKING);
	spriteAnimation->Play(ActorState::WALKING);
	spriteAnimation->UpdatePlaySpeed(0.f);

	weapon->Init();
	weapon->Refresh();

	inputDevice = nullptr;
}

void Player::Update(float dt)
{
	if (IsAlive())
	{
		if (IsActing())
		{
			this->HandleWalk(dt);
			this->HandleWeapons();
		}
	}

	Actor::Update(dt);
}

void Player::HandleWalk(float dt)
{
	if (GetCurrentState() != ActorState::WALKING || inputDevice == nullptr)
		return;

	Vector2D moveDirection = Vector2D(GameInput::GetFirstAxesX(inputDevice), GameInput::GetFirstAxesY(inputDevice));
	Vector2D rotateDirection = Vector2D(GameInput::GetSecondAxesX(inputDevice), GameInput::GetSecondAxesY(inputDevice));

	if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
	{
		Vector2D playerScreen;
		Camera2D::g_pActiveCamera->WorldSpaceToVirtualScreenSpace(position, playerScreen);
		rotateDirection -= playerScreen;
	}

	//// update the velocity of the player
	if (moveDirection.LengthSq() > 0)
		Actor::Move(moveDirection);

	// update the rotation of the player
	if (rotateDirection.LengthSq() > 0)
		Actor::Rotate(rotateDirection, dt);

	if (spriteAnimation->IsPlaying(ActorState::WALKING))
	{
		Vector2D lookDirection = Vector2D(0, 1).Rotate(rotation);
		moveDirection.SafeNormalize();
		float playSpeed = moveDirection.Dot(lookDirection);

		spriteAnimation->UpdatePlaySpeed(playSpeed);
	}
}

void Player::HandleWeapons()
{
	if (inputDevice == nullptr)
		return;

	int indexInventory = GameInput::CheckChangeWeapon(inputDevice);

	switch (indexInventory)
	{
	case 1:
		weapon->ChangeWeapon(WeaponType::Gun);
		break;
	case 2:
		weapon->ChangeWeapon(WeaponType::AssaultRifle);
		break;
	case 3:
		weapon->ChangeWeapon(WeaponType::RocketLauncher);
		break;
	case 4:
		weapon->ChangeWeapon(WeaponType::GaussRifle);
		break;
	default:
		break;
	}

	if (GameInput::CheckFire(inputDevice))
		weapon->FireBullet();
}

void Player::UpdatePhysics(float dt)
{
	Actor::UpdatePhysics(dt);

	// Check for player exiting terrain
	const float eps = 1e-6f;
	const Vector2D playerHalfSize = size / 2;
	const Vector2D playingAreaHalf = Game::g_pGame->terrain->playingArea / 2;

	const Vector2D left_bottom = position - playerHalfSize;
	const Vector2D right_top = position + playerHalfSize;

	if (left_bottom.x + playingAreaHalf.x <= -eps)
		position.x = -playingAreaHalf.x + playerHalfSize.x;

	if (right_top.x - playingAreaHalf.x >= eps)
		position.x = playingAreaHalf.x - playerHalfSize.x;

	if (left_bottom.y + playingAreaHalf.y <= -eps)
		position.y = -playingAreaHalf.y + playerHalfSize.y;

	if (right_top.y - playingAreaHalf.y >= eps)
		position.y = playingAreaHalf.y - playerHalfSize.y;
}

void Player::OnEnd()
{
	ActorState currentState = this->GetCurrentState();

	if (currentState != ActorState::DYING && currentState != ActorState::ENDING)
	{
		if (inputDevice != nullptr)
		{
			GameInput::DeactivateDevice(inputDevice);
		}

		Actor::OnEnd();
	}
	else if (currentState != ActorState::ENDING)
	{
		this->UpdateState(ActorState::ENDING);

		Controller* fadeController = this->FindController(ObjectType::FadeAnimation);

		if (fadeController != nullptr && fadeController->IsTypeOf(ObjectType::AnimationController))
		{
			AnimationController* fadeAnimation = static_cast<AnimationController*>(fadeController);
			fadeAnimation->SetAnimationState(AnimationController::AnimationState::BACKWARD);
			fadeAnimation->StartAnimation();
		}
	}
	else
		PhysicsObject::OnEnd();
}

void Player::SetInputDevice(const InputDevice* setInputDevice)
{
	if (setInputDevice != nullptr)
	{
		this->inputDevice = setInputDevice;

		color = GameInput::GetColor(setInputDevice);
		weapon->Refresh();
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE