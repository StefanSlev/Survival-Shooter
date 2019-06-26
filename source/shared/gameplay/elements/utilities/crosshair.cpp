#include "shared/gameplay/elements/utilities/crosshair.h"
#include "shared/gameplay/engine/sprite.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Crosshair::Crosshair(Sprite* sprite) : WorldObject(sprite) {

	Init();
}

void Crosshair::Init() {

	size = 0.5f;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE