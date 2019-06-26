#ifndef SHARED_GAMEPLAY_ELEMENTS_CAMERA_CONTROLLER
#define SHARED_GAMEPLAY_ELEMENTS_CAMERA_CONTROLLER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/animations/animation_controller.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// This is a controller for the camera in the game
// Use it to make the camera follow the players (they need to be
// added as followed objects with AddFollow method)
// It also offers ZOOM_IN/ZOOM_OUT functionality
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Object;

class CameraController final : public AnimationController
{
public:
	CameraController();

	ObjectType GetType() const override { return ObjectType::CameraController; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::CameraController) || AnimationController::IsTypeOf(type); }

	void Update(float dt) override;
	void UpdateAnimation(float dt) override;
	void OnAnimationEnd() override;

	void Init();

private:
	void SetObject(Object * _object) override;

	enum CameraState {
		ZOOMED_IN,
		ZOOMED_OUT
	};
	CameraState cameraState;

	Vector2D dynamicZoomedInPos;
	Vector2D zoomedInPos;
	float zoomedInScale;

	void ComputeZoomedInProp();
	float maxScale;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_CAMERA_CONTROLLER