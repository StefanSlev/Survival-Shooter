#ifndef SHARED_GAMEPLAY_ENGINE_CONSTANTS
#define SHARED_GAMEPLAY_ENGINE_CONSTANTS

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// This file contains some global constants, that are used throughout the application.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

constexpr double M_PI = 3.14159265359f;
constexpr double M_EULER = 2.71828182846f;

constexpr int WINDOW_OFFSET_X = 225;
constexpr int WINDOW_OFFSET_Y = 150;

constexpr int REFERENCE_RESOLUTION_Y = 600;

constexpr unsigned int FRAMES_PER_SECOND = 60;
constexpr double FRAME_TIME = 1.f / FRAMES_PER_SECOND;

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_CONSTANTS