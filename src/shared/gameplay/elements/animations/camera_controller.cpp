#include "shared/gameplay/elements/animations/camera_controller.h"
#include "shared/gameplay/elements/terrain.h"
#include "shared/gameplay/elements/game.h"
#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

CameraController::CameraController() : AnimationController()
{
	object = nullptr;
	maxScale = 1.f;
}

void CameraController::SetObject(Object * _object)
{
	AnimationController::SetObject(_object);
	
	if (object->IsTypeOf(ObjectType::Camera))
	{
		const float cameraSizeY = static_cast<Camera2D*> (object)->defaultScreenSizeInWorldUnits.y;

		const Terrain* terrain = Game::g_pGame->terrain;
		const float playAreaMarginY = terrain->playingAreaFence->size.y + 2 * terrain->marginArea.y;

		maxScale = playAreaMarginY / cameraSizeY;
	}
}

void CameraController::Init()
{
	zoomedInPos = 0.f;
	dynamicZoomedInPos = 0.f;
	zoomedInScale = 1.f;

	cameraState = ZOOMED_IN;
	animationState = AnimationState::FORWARD;
	animationTime = 0.f;
	animate = false;
}

void CameraController::UpdateAnimation(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// scale the camera during animation
	object->scale = Lerp(maxScale, zoomedInScale, animationTime);

	// set the position of the camera during animation
	object->position = Lerp(0.f, dynamicZoomedInPos, animationTime);
}

void CameraController::OnAnimationEnd()
{
	switch (cameraState)
	{
	case ZOOMED_IN:
		cameraState = ZOOMED_OUT;
		break;
	case ZOOMED_OUT:
		cameraState = ZOOMED_IN;
		break;
	default:
		break;
	}
}

void CameraController::ComputeZoomedInProp()
{
	const float eps = 1e-6f;
	Vector2D cameraSize = static_cast<Camera2D*> (object)->defaultScreenSizeInWorldUnits;

	Vector2D barycenter = 0.f;
	unsigned int nrFollowed = 0;

	const auto& players = Game::g_pGame->players;

	for (unsigned int i = 0; i < players.size(); ++i)
	{
		Object* player = players[i];

		if (player && player->active)
		{
			barycenter += player->position;
			nrFollowed++;
		}
	}
		
	if (nrFollowed == 0)
		return;

	barycenter /= (nrFollowed * 1.f);

	// Compute zoomed-in position of the camera
	Vector2D cameraLeftBottom = barycenter - cameraSize / 2;
	Vector2D cameraRightTop = barycenter + cameraSize / 2;

	Terrain* terrain = Game::g_pGame->terrain;
	Vector2D halfPlayAreaMargin = terrain->playingAreaFence->size / 2 + terrain->marginArea;

	if (cameraLeftBottom.x + halfPlayAreaMargin.x > -eps && cameraRightTop.x - halfPlayAreaMargin.x < eps)
		zoomedInPos.x = barycenter.x;

	if (cameraLeftBottom.y + halfPlayAreaMargin.y > -eps && cameraRightTop.y - halfPlayAreaMargin.y < eps)
		zoomedInPos.y = barycenter.y;

	// Compute zoomed-in scale for the camera
	if (nrFollowed >= 2)
	{
		Vector2D box = terrain->playingArea;

		// corners of the bounding box of all the followed objects
		Vector2D left_bottom = box;
		Vector2D right_top = Vector2D(0) - box;

		// half size of the leftmost and lowermost followed objects
		Vector2D missSizeLL = 0.f;
		// half size of the rightmost and uppermost followed objects
		Vector2D missSizeRU = 0.f;

		for (unsigned int i = 0; i < players.size(); ++i)
		{
			Object* player = players[i];

			if (player && player->active)
			{
				if (player->position.x - left_bottom.x <= -eps)
					left_bottom.x = player->position.x, missSizeLL.x = player->size.x * 0.5f;

				if (player->position.y - left_bottom.y <= -eps)
					left_bottom.y = player->position.y, missSizeLL.y = player->size.y * 0.5f;

				if (player->position.x - right_top.x >= eps)
					right_top.x = player->position.x, missSizeRU.x = player->size.x * 0.5f;

				if (player->position.y - right_top.y >= eps)
					right_top.y = player->position.y, missSizeRU.y = player->size.y * 0.5f;
			}
		}

		box -= (missSizeLL + missSizeRU);
		Vector2D scales = (right_top - left_bottom) / box;
		float scaleProc = fmaxf(scales.x, scales.y);

		zoomedInScale = Lerp(1.f, maxScale, scaleProc);
		dynamicZoomedInPos = Lerp(zoomedInPos, 0.f, scaleProc);
	}
	else
	{
		dynamicZoomedInPos = zoomedInPos;
		zoomedInScale = 1.f;
	}
}

void CameraController::Update(float dt)
{
	if (Game::g_pGame->gameState == Game::GameState::InGame)
	{
		// calculate the next position and scale of the camera in zoomed-in view
		this->ComputeZoomedInProp();
		
		if (!animate && cameraState == ZOOMED_IN)
		{
			object->position = dynamicZoomedInPos;
			object->scale = zoomedInScale;
		}
	}

	AnimationController::Update(dt);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE