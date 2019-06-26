#include "shared/gameplay/engine/camera.h"
#include "shared/gameplay/engine/engine.h"
#include "shared/gameplay/engine/menu_item.h"
#include "shared/gameplay/engine/constants.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Camera2D *Camera2D::g_pActiveCamera = NULL;

Camera2D::Camera2D(float size) : Object ()
{
	if (!g_pActiveCamera)
		g_pActiveCamera = this;

	// retrieve viewport size (screen size for us)
	D3DVIEWPORT9 viewport;
	Engine::g_pd3dDevice->GetViewport(&viewport);

	screenSize.x = (float)viewport.Width;
	screenSize.y = (float)viewport.Height;
	aspectRatio = screenSize.x / screenSize.y;

	orthographicSize = size;

	screenToWorldScale = screenSize.y / orthographicSize / 2;
	worldCenterOffset.Set(screenSize.x / 2, screenSize.y / 2);

	defaultScreenSizeInWorldUnits = screenSize / screenToWorldScale;

	virtualScreenRatio = screenSize.y / REFERENCE_RESOLUTION_Y;
	virtualScreenSize = Vector2D(REFERENCE_RESOLUTION_Y * screenSize.x / screenSize.y, (float)REFERENCE_RESOLUTION_Y);
}

Camera2D::~Camera2D()
{
	if (g_pActiveCamera == this)
		g_pActiveCamera = NULL;
}

void Camera2D::WorldSpaceToScreenSpace(Object *object, Vector2D & outPos, Vector2D & outSize, float & outRotation)
{
	// Through this World Space Transform do the following:
	//   - center the world in the initial center of the screen (previously was in the top left corner)
	//   - make the Y axis grow upwards (previously was growing downwards)
	//   - make world units dependent of the camera's orthographicSize (previousely were screen pixels)
	//   - take into consideration camera's position, rotation and zoom (1/scale)

	// reverse the direction of the Y axis while considering camera rotation
	if (rotation != 0)
		outPos = object->global.position.Rotate(rotation);
	else
		outPos.Set(object->global.position.x, object->global.position.y);
	outPos -= position;
	outPos.y = -outPos.y; // make Y axis grow upwards
	outPos *= screenToWorldScale; // make world units dependent of the camera's orthographicSize
	outPos /= scale;
	outPos += worldCenterOffset; // offset for the world center position

	// add the inverse camera rotation to the object rotation; compensate for the switch of the Y axis
	outRotation = -(object->global.rotation - rotation);

	// compute the size of the object, considering both the object's scale and camera's zoom (1/scale)
	outSize = object->size;
	outSize *= object->global.scale;
	outSize *= screenToWorldScale;
	outSize /= scale;
}

void Camera2D::WorldSpaceToVirtualScreenSpace(Vector2D & worldPos, Vector2D & screenPos)
{
	screenPos = worldPos - position;
	screenPos *= screenToWorldScale;
	screenPos /= scale;
	screenPos /= virtualScreenRatio;
}

void Camera2D::VirtualScreenToScreenSpace(MenuItem *menu, Vector2D & outPos, Vector2D & outSize, float & outRotation)
{
	outPos = menu->global.position * virtualScreenRatio;
	outPos.y = -outPos.y;
	outPos += worldCenterOffset;
	outSize = menu->size * virtualScreenRatio;
	outRotation = -menu->global.rotation;
}

void Camera2D::VirtualScreenToScreenSpace(Vector2D & virtualPos, Vector2D & screenPos)
{
	screenPos = virtualPos * virtualScreenRatio;
	screenPos.y = -screenPos.y;
	screenPos += worldCenterOffset;
}

void Camera2D::ScreenToVirtualScreenSpace(Vector2D & screenPos, Vector2D & virtualPos)
{
	virtualPos = screenPos - worldCenterOffset;
	virtualPos.y = -virtualPos.y;
	virtualPos /= virtualScreenRatio;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE