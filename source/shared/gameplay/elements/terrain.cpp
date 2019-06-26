#include "shared/gameplay/elements/terrain.h"
#include "shared/gameplay/engine/sprite_animation_controller.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* Terrain::terrainFileName = "Data\\Images\\terrain.png";
const char* Terrain::playAreaAnim = "Data\\Anims\\anim_play_area.png";

Terrain::Terrain() : WorldObject()
{
	Init();
}

void Terrain::Init()
{
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	pSprite = resourceManager.GetSprite(ResourceType::IMAGE, terrainFileName);
	
	// terrain properties
	Vector2D cameraSize = Camera2D::g_pActiveCamera->defaultScreenSizeInWorldUnits;
	size = cameraSize * (TERRAIN_SIZE_Y / cameraSize.y);

	playingArea = { 60.f, 40.f };
	marginArea = 5.f;

	// play area properties
	playingAreaFence = new WorldObject();

	playingAreaFence->size = playingArea + Vector2D(2.f, 1.3f); // add the fence's width
	playingAreaFence->color = sColor(100, 255, 255, 255).GetValue();

	playingAreaFence->spriteTopLeftUV = 0;
	playingAreaFence->spriteBottomRightUV = { 0.25f, 0.25f };
	this->AddChild(playingAreaFence);

	// add fence animation
	SpriteAnimationController<PlayAreaAnimation>* spriteAnimation = new SpriteAnimationController<PlayAreaAnimation>();
	SpriteAnimationInfo<PlayAreaAnimation>* animationInfo = new SpriteAnimationInfo<PlayAreaAnimation>();

	animationInfo->animationType = PlayAreaAnimation::ANIMATE;
	animationInfo->animationSprite = resourceManager.GetSprite(ResourceType::ANIMATION, playAreaAnim);
	animationInfo->frameSize = 1024;
	animationInfo->startIndex = 0;
	animationInfo->animationLength = 16;
	animationInfo->fps = 60.f;
	animationInfo->looping = true;

	spriteAnimation->Load(animationInfo);
	playingAreaFence->AddController(spriteAnimation);
	spriteAnimation->Play(PlayAreaAnimation::ANIMATE);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE