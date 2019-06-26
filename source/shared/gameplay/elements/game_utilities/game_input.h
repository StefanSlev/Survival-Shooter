#ifndef SHARED_GAMEPLAY_ELEMENTS_GAME_INPUT
#define SHARED_GAMEPLAY_ELEMENTS_GAME_INPUT

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Class used to simplify game input processing.
// It's based on Input file
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

enum class InputType
{	
	GAMEPAD,
	KEYBOARD_MOUSE
};

class GameInput;

struct InputDevice
{
	InputType inputType;

protected:
	bool isConnected;
	bool isUsed;

	InputDevice(InputType inputType) : inputType(inputType) {
		this->isConnected = false;
		this->isUsed = false;
	}

	friend class GameInput;
};

struct Gamepad : public InputDevice
{
	int id_gamepad;
	Gamepad(int id_gamepad) : InputDevice(InputType::GAMEPAD), id_gamepad(id_gamepad) {}
};

struct KeyboardMouse : public InputDevice 
{
	KeyboardMouse() : InputDevice(InputType::KEYBOARD_MOUSE) {}
};

struct GameAction {

	WORD gamepad_button;
	int keyboard_key;
};

class GameInput
{
public:
	static GameInput& GetInstance()
	{
		static GameInput instance;
		return instance;
	}

	~GameInput();

	// devices handle methods
	bool IsKeyboardMouseConnected() const;
	bool IsGamepadConnected(int gamepadIndex) const;
	
	const InputDevice* ConnectKeyboardMouse();
	void DisconnectKeyboardMouse();
	void DeactivateKeyboardMouse();
	void ActivateKeyboardMouse();

	const InputDevice* ConnectGamepad(int gamepadIndex);
	void DisconnectGamepad(int gamepadIndex);
	void DeactivateGamepad(int gamepadIndex);
	void ActivateGamepad(int gamepadIndex);

	void ResetDevices();

	// in-game actions
	bool CheckLeaderboardToggle() const;
	bool CheckCameraToggle() const;

	// methods to use in player class
	static void DisconnectDevice(const InputDevice* inputDevice);
	static void DeactivateDevice(const InputDevice* inputDevice);
	static void ActivateDevice(const InputDevice* inputDevice);

	static float GetFirstAxesX(const InputDevice* inputDevice);
	static float GetFirstAxesY(const InputDevice* inputDevice);

	static float GetSecondAxesX(const InputDevice* inputDevice);
	static float GetSecondAxesY(const InputDevice* inputDevice);

	// returns the index of the weapon in the inventory
	static int CheckChangeWeapon(const InputDevice* inputDevice);
	static bool CheckFire(const InputDevice* inputDevice);

	// return associated color for the given device
	static long GetColor(const InputDevice* inputDevice);

private:
	// available devices to use in the game
	Gamepad* gamepads[XUSER_MAX_COUNT];
	KeyboardMouse* keyboardMouse;

	// internal info for action-keys mapping
	static const GameAction leaderboardAction;
	static const GameAction cameraToggleAction;

	bool CheckAction(const GameAction& action) const;

	/** Singleton **/
	GameInput();
	GameInput(const GameInput&) = delete;
	void operator= (const GameInput&) = delete;

	static const long gamepadsColors[XUSER_MAX_COUNT];
	static const long keyboardMouseColor;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_GAME_INPUT