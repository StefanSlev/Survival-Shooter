#ifndef SHARED_GAMEPLAY_ENGINE_CAMERA
#define SHARED_GAMEPLAY_ENGINE_CAMERA

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// Class that represents a game Camera.
// The Camera handles the transformation between 3 different (mathematical) spaces used in the game:
//	- the World Space (used to make the game resolution independent)
//	- the Virtual Screen Space (used to make the menus resolution independent)
//	- and the default (used by DirectX to draw) Screen Space
// Inside the game, you should only work with World & Virtual Screen Spaces.
// Game objects are living in the World Space & menus in the Virtual Screen Space.
// At render time, the renderer is transforming in the background all the objects (game & menus) from
//	thier space to the Screen Space, to prepare them for drawing.
//
// Note: the global accessible Camera2D::g_pActiveCamera represents the current active camera used by the game
// 
// Note: the Camera is derived from Object, so it has a Transform - as a result, you can move, rotate
//	or scale (zoom) the camera
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class MenuItem;

class Camera2D : public Object
{
public:
	static Camera2D *g_pActiveCamera;

	// Initialize the camera with the (orthographic) size = half the size of the vertical viewing volume
	// In other words, from top to bottom of the window, there are size * 2 space units
	// The number of space units on the horizontal depends on the aspect ratio of the window
	Camera2D(float size = 10);
	~Camera2D();

	// Use this to transform game objects from world space into camera space before rendering
	void WorldSpaceToScreenSpace(Object *object, Vector2D &outPos, Vector2D &outSize, float &outRotation);
	void WorldSpaceToVirtualScreenSpace(Vector2D &worldPos, Vector2D &screenPos);

	// Use this to transform menu items from the virtual screen space to camera space before rendering
	void VirtualScreenToScreenSpace(MenuItem *menu, Vector2D &outPos, Vector2D &outSize, float &outRotation);
	void VirtualScreenToScreenSpace(Vector2D & virtualPos, Vector2D & screenPos);

	void ScreenToVirtualScreenSpace(Vector2D & screenPos, Vector2D & virtualPos);

	virtual ObjectType GetType() const { return ObjectType::Camera; }
	virtual bool IsTypeOf(ObjectType type) const { return (type == ObjectType::Camera) || Object::IsTypeOf(type); }

	Vector2D screenSize;
	Vector2D virtualScreenSize;

	Vector2D defaultScreenSizeInWorldUnits;
	float screenToWorldScale;
	float aspectRatio;
	float virtualScreenRatio;

protected:
	float orthographicSize;
	Vector2D worldCenterOffset;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_CAMERA