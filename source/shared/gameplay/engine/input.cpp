#include "shared/gameplay/engine/input.h"
#include "shared/gameplay/engine/engine.h"
#include "shared/gameplay/engine/camera.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const float DEADZONE_LEFT = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32767.f;
const float DEADZONE_RIGHT = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32767.f;

XINPUT_STATE Input::previousGamepadState[XUSER_MAX_COUNT];
XINPUT_STATE Input::gamepadState[XUSER_MAX_COUNT];
BYTE Input::keyboardState[256];
BYTE Input::previousKeyboardState[256];

void Input::UpdateInputState()
{
	// update the keyboard state
	memcpy_s(previousKeyboardState, 256, keyboardState, 256);
	GetKeyboardState(keyboardState);

	// update the gamepad state
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		previousGamepadState[i] = gamepadState[i];
		ZeroMemory(&gamepadState[i], sizeof(XINPUT_STATE));
		XInputGetState(i, &gamepadState[i]);
	}
}

// returns true if the specified key is held down
bool Input::IsKeyDown(int key)
{
	return keyboardState[key] & 0x80;
}

// returns true if the specified key was just pressed
bool Input::IsKeyPressed(int key)
{
	bool pressed = (keyboardState[key] & 0x80) && !(previousKeyboardState[key] & 0x80);
	previousKeyboardState[key] = keyboardState[key];
	return pressed;
}

shared::gameplay::Vector2D Input::GetMousePosition()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(Engine::g_hWnd, &p);
	shared::gameplay::Vector2D screenMousePos = { (float)p.x, (float)p.y };
	shared::gameplay::Vector2D virtualMousePos;
	Camera2D::g_pActiveCamera->ScreenToVirtualScreenSpace(screenMousePos, virtualMousePos);
	return virtualMousePos;
}

bool Input::IsGamepadButtonDown(WORD button, int userIndex)
{
	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
		return ((gamepadState[userIndex].Gamepad.wButtons & button) != 0);

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
			if (IsGamepadButtonDown(button, i))
				return true;
	}

	return false;
}

bool Input::IsGamepadButtonPressed(WORD button, int userIndex)
{
	bool ret = false;

	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
	{
		ret = ((gamepadState[userIndex].Gamepad.wButtons & button) != 0) && ((previousGamepadState[userIndex].Gamepad.wButtons & button) == 0);
		if (ret)
			previousGamepadState[userIndex].Gamepad.wButtons |= button;
		return ret;
	}

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
			ret = ret || IsGamepadButtonPressed(button, i);
		return ret;
	}

	return false;
}

float Input::GetGamepadLeftTrigger(int userIndex)
{
	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
		return gamepadState[userIndex].Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0 : gamepadState[userIndex].Gamepad.bLeftTrigger / 255.f;

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			float value = GetGamepadLeftTrigger(i);
			if (value != 0)
				return value;
		}
	}

	return 0;
}

float Input::GetGamepadRightTrigger(int userIndex)
{
	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
		return gamepadState[userIndex].Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0 : gamepadState[userIndex].Gamepad.bRightTrigger / 255.f;

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			float value = GetGamepadRightTrigger(i);
			if (value != 0)
				return value;
		}
	}

	return 0;
}

float Input::GetGamepadLeftStickX(int userIndex)
{
	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
	{
		float valueX = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbLX / 32767.f);
		float valueY = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbLY / 32767.f);
		return (fabsf(valueX) < DEADZONE_LEFT && fabsf(valueY) < DEADZONE_LEFT) ? 0 : valueX;
	}

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			float value = GetGamepadLeftStickX(i);
			if (value != 0)
				return value;
		}
	}

	return 0;
}

float Input::GetGamepadLeftStickY(int userIndex)
{
	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
	{
		float valueX = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbLX / 32767.f);
		float valueY = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbLY / 32767.f);
		return (fabsf(valueX) < DEADZONE_LEFT && fabsf(valueY) < DEADZONE_LEFT) ? 0 : valueY;
	}

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			float value = GetGamepadLeftStickY(i);
			if (value != 0)
				return value;
		}
	}

	return 0;
}

float Input::GetGamepadRightStickX(int userIndex)
{
	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
	{
		float valueX = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbRX / 32767.f);
		float valueY = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbRY / 32767.f);
		return (fabsf(valueX) < DEADZONE_RIGHT && fabsf(valueY) < DEADZONE_RIGHT) ? 0 : valueX;
	}

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			float value = GetGamepadRightStickX(i);
			if (value != 0)
				return value;
		}
	}

	return 0;
}

float Input::GetGamepadRightStickY(int userIndex)
{
	if (userIndex >= 0 && userIndex < XUSER_MAX_COUNT)
	{
		float valueX = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbRX / 32767.f);
		float valueY = fmaxf(-1, gamepadState[userIndex].Gamepad.sThumbRY / 32767.f);
		return (fabsf(valueX) < DEADZONE_RIGHT && fabsf(valueY) < DEADZONE_RIGHT) ? 0 : valueY;
	}

	if (userIndex == -1)
	{
		for (int i = 0; i < XUSER_MAX_COUNT; i++)
		{
			float value = GetGamepadRightStickY(i);
			if (value != 0)
				return value;
		}
	}

	return 0;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE