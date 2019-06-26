#ifndef SHARED_GAMEPLAY_ENGINE_TRANSFORM2D
#define SHARED_GAMEPLAY_ENGINE_TRANSFORM2D

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// Base class from that represents a 2D spatial transformation (position, rotation and scale)
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Transform2D
{
public:
	Transform2D(Vector2D _position = { 0 , 0 }, float _rotation = 0, Vector2D _scale = { 1, 1 })
	{
		position = _position;
		rotation = _rotation;
		scale = _scale;
	}

	virtual void Init();

	Vector2D		position;
	float			rotation;
	Vector2D		scale;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_TRANSFORM2D