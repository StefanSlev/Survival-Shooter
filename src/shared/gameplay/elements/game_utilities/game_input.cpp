#include "shared/gameplay/elements/game_utilities/game_input.h"
#include "shared/gameplay/elements/weapons/weapon_info.h"
#include "shared/gameplay/engine/utils.h"
#include "shared/gameplay/engine/input.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const GameAction GameInput::pauseAction = { XINPUT_GAMEPAD_START, VK_SPACE };
const GameAction GameInput::cameraToggleAction = { XINPUT_GAMEPAD_BACK, VK_BACK };
const GameAction GameInput::resetAction = { XINPUT_GAMEPAD_X, 'R' };
const GameAction GameInput::saveGameAction = {XINPUT_GAMEPAD_A, 'S'};
const GameAction GameInput::loadGameAction = {XINPUT_GAMEPAD_B, 'L'};

const long GameInput::gamepadsColors[XUSER_MAX_COUNT] = { sColor(255, 0, 204, 153).GetValue(), sColor(255, 255, 255, 102).GetValue(), 
sColor(255, 204, 153, 255).GetValue(), sColor(255, 0, 153, 204).GetValue() };
const long GameInput::keyboardMouseColor = sColor(255, 255, 102, 102).GetValue();

GameInput::GameInput() 
{
	keyboardMouse = new KeyboardMouse();

	for (unsigned int i = 0; i < XUSER_MAX_COUNT; i++)
		gamepads[i] = new Gamepad(i);
}

GameInput::~GameInput()
{
	if (keyboardMouse)
		delete keyboardMouse;

	for (unsigned int i = 0; i < XUSER_MAX_COUNT; i++)
		if (gamepads[i])
			delete gamepads[i];
}

void GameInput::ResetDevices()
{
	this->DisconnectKeyboardMouse();

	for (unsigned int gamepadIndex = 0; gamepadIndex < XUSER_MAX_COUNT; gamepadIndex++)
		this->DisconnectGamepad(gamepadIndex);
}

bool GameInput::IsKeyboardMouseConnected() const
{
	return keyboardMouse->isConnected;
}

bool GameInput::IsGamepadConnected(int gamepadIndex) const
{
	if (gamepadIndex < 0 || gamepadIndex >= XUSER_MAX_COUNT)
		return false;

	return gamepads[gamepadIndex]->isConnected;
}

const InputDevice* GameInput::ConnectKeyboardMouse()
{
	if (!keyboardMouse->isConnected) {

		keyboardMouse->isConnected = true;
		keyboardMouse->isUsed = true;
		return keyboardMouse;
	}

	return nullptr;
}

const InputDevice* GameInput::ConnectGamepad(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= XUSER_MAX_COUNT)
		return nullptr;

	if (!gamepads[gamepadIndex]->isConnected) {

		gamepads[gamepadIndex]->isConnected = true;
		gamepads[gamepadIndex]->isUsed = true;
		return gamepads[gamepadIndex];
	}

	return nullptr;
}

void GameInput::DisconnectKeyboardMouse()
{
	keyboardMouse->isConnected = false;
	keyboardMouse->isUsed = false;
}

void GameInput::DisconnectGamepad(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= XUSER_MAX_COUNT)
		return;

	gamepads[gamepadIndex]->isConnected = false;
	gamepads[gamepadIndex]->isUsed = false;
}

void GameInput::DeactivateKeyboardMouse()
{
	if (!keyboardMouse->isConnected)
		return;

	keyboardMouse->isUsed = false;
}

void GameInput::DeactivateGamepad(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= XUSER_MAX_COUNT || !gamepads[gamepadIndex]->isConnected)
		return;

	gamepads[gamepadIndex]->isUsed = false;
}

void GameInput::ActivateKeyboardMouse()
{
	if (!keyboardMouse->isConnected)
		return;

	keyboardMouse->isUsed = true;
}

void GameInput::ActivateGamepad(int gamepadIndex)
{
	if (gamepadIndex < 0 || gamepadIndex >= XUSER_MAX_COUNT || !gamepads[gamepadIndex]->isConnected)
		return;

	gamepads[gamepadIndex]->isUsed = true;
}

bool GameInput::CheckAction(const GameAction& action) const {

	if (keyboardMouse->isUsed && Input::IsKeyPressed(action.keyboard_key))
		return true;

	for (unsigned int index = 0; index < XUSER_MAX_COUNT; index++)
		if (gamepads[index]->isUsed && Input::IsGamepadButtonPressed(action.gamepad_button, index))
			return true;

	return false;
}

bool GameInput::CheckPauseToggle() const
{
	return this->CheckAction(pauseAction);
}

bool GameInput::CheckCameraToggle() const
{
	return this->CheckAction(cameraToggleAction);
}

bool GameInput::CheckReset() const
{
	return this->CheckAction(resetAction);
}

bool GameInput::CheckSaveGame() const
{
	return this->CheckAction(saveGameAction);
}

bool GameInput::CheckLoadGame() const
{
	return this->CheckAction(loadGameAction);
}

void GameInput::DisconnectDevice(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isConnected)
		return;

	GameInput& gameInput = GameInput::GetInstance();

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		gameInput.DisconnectGamepad(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
	{
		gameInput.DisconnectKeyboardMouse();
	}
}

void GameInput::DeactivateDevice(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isConnected)
		return;

	GameInput& gameInput = GameInput::GetInstance();

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		gameInput.DeactivateGamepad(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
	{
		gameInput.DeactivateKeyboardMouse();
	}
}

void GameInput::ActivateDevice(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isConnected)
		return;

	GameInput& gameInput = GameInput::GetInstance();

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		gameInput.ActivateGamepad(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
	{
		gameInput.ActivateKeyboardMouse();
	}
}

float GameInput::GetFirstAxesX(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isUsed)
		return 0.f;

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		return Input::GetGamepadLeftStickX(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
	{
		float directionX = 0.f;

		if (Input::IsKeyDown('A'))
			directionX -= 1.f;
		if (Input::IsKeyDown('D'))
			directionX += 1.f;

		return directionX;
	}

	return 0.f;
}

float GameInput::GetFirstAxesY(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isUsed)
		return 0.f;

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		return Input::GetGamepadLeftStickY(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
	{
		float directionY = 0.f;

		if (Input::IsKeyDown('S'))
			directionY -= 1.f;
		if (Input::IsKeyDown('W'))
			directionY += 1.f;

		return directionY;
	}

	return 0.f;
}

float GameInput::GetSecondAxesX(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isUsed)
		return 0.f;

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		return Input::GetGamepadRightStickX(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
		return Input::GetMousePosition().x;

	return 0.f;
}

float GameInput::GetSecondAxesY(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isUsed)
		return 0.f;

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		return Input::GetGamepadRightStickY(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
		return Input::GetMousePosition().y;

	return 0.f;
}

bool GameInput::CheckFire(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isUsed)
		return false;

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		return Input::GetGamepadRightTrigger(gamepadIndex);
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
		return Input::IsKeyDown(VK_LBUTTON);

	return false;
}

int GameInput::CheckChangeWeapon(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isUsed)
		return 0;

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;

		if (Input::IsGamepadButtonPressed(XINPUT_GAMEPAD_X, gamepadIndex))
			return 1;
		else if (Input::IsGamepadButtonPressed(XINPUT_GAMEPAD_A, gamepadIndex))
			return 2;
		else if (Input::IsGamepadButtonPressed(XINPUT_GAMEPAD_B, gamepadIndex))
			return 3;
		else if (Input::IsGamepadButtonPressed(XINPUT_GAMEPAD_Y, gamepadIndex))
			return 4;

		return 0;
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
	{
		for (int index = 1; index <= INVENTORY_SIZE; index++)
			if (Input::IsKeyPressed('0' + index))
				return index;
	}

	return 0;
}

long GameInput::GetColor(const InputDevice* inputDevice)
{
	if (!inputDevice || !inputDevice->isUsed)
		return 0;

	if (inputDevice->inputType == InputType::GAMEPAD)
	{
		int gamepadIndex = static_cast<const Gamepad*>(inputDevice)->id_gamepad;
		return gamepadsColors[gamepadIndex];
	}
	else if (inputDevice->inputType == InputType::KEYBOARD_MOUSE)
		return keyboardMouseColor;

	return 0;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE