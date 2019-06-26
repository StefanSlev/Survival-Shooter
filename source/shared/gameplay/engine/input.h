#ifndef SHARED_GAMEPLAY_ENGINE_INPUT
#define SHARED_GAMEPLAY_ENGINE_INPUT

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/vector2d.h"

// -------------------------------------------------------------------------------------
// Class used to simplify input processing in the game.
// It provides both keyboard and gamepad support
//
// Note: It can be extended to provide mouse, as well as multiple gamepads support.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Input
{
public:
	// call this at the beginning of the frame to update all the gamepads & keyboard states
	static void UpdateInputState();

	// returns true if the specified key is held down (aka VK_SPACE or 'X')
	static bool IsKeyDown(int key);
	// returns true if the specified key was just pressed
	static bool IsKeyPressed(int key);
	// returns the mouse position in virtual screen space
	static Vector2D GetMousePosition();

	// returns true if the specified button is held down (aka XINPUT_GAMEPAD_A or
	//	XINPUT_GAMEPAD_DPAD_LEFT or XINPUT_GAMEPAD_LEFT_SHOULDER or XINPUT_GAMEPAD_LEFT_THUMB or XINPUT_GAMEPAD_BACK)
	// pass the userIndex = gamepad port number (0-3) from which you want to read the state
	// pass userIndex = -1 if you want to test the input from all gamepad ports
	static bool IsGamepadButtonDown(WORD button, int userIndex = -1);
	static bool IsGamepadButtonPressed(WORD button, int userIndex = -1);
	// functions for pooling triggers (returns value in the range [0, 1])
	static float GetGamepadLeftTrigger(int userIndex = -1);
	static float GetGamepadRightTrigger(int userIndex = -1);
	// functions for pooling sticks (returns value in the range [-1, 1])
	// negative values signify down or to the left, positive values signify up or to the right
	static float GetGamepadLeftStickX(int userIndex = -1);
	static float GetGamepadLeftStickY(int userIndex = -1);
	static float GetGamepadRightStickX(int userIndex = -1);
	static float GetGamepadRightStickY(int userIndex = -1);

protected:
	static XINPUT_STATE previousGamepadState[XUSER_MAX_COUNT];
	static XINPUT_STATE gamepadState[XUSER_MAX_COUNT];
	static BYTE			keyboardState[256];
	static BYTE			previousKeyboardState[256];
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_INPUT