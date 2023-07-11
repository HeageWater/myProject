#pragma once
#include<dinput.h>
#include<DirectXMath.h>
#include<wrl.h>
#include "Vector2.h"
#include "Vector3.h"
#include <xinput.h>
#pragma comment (lib, "xinput.lib")

enum ControllerButton
{
	B = XINPUT_GAMEPAD_B,
	A = XINPUT_GAMEPAD_A,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	LT,
	RT
};

enum ControllerStick
{
	L_UP, L_DOWN, L_LEFT, L_RIGHT,
	R_UP, R_DOWN, R_LEFT, R_RIGHT, XBOX_STICK_NUM
};

class Controller
{
private:

	Microsoft::WRL::ComPtr<IDirectInputDevice8> controller = nullptr;
	XINPUT_STATE controllerState{};
	XINPUT_STATE oldControllerState{};

	//ç≈çÇì¸óÕã≠ìx
	const float STICK_INPUT_MAX = 32768.0f;

public:

	void Update();

	bool ButtonTriggerPush(ControllerButton button);

	bool ButtonKeepPush(ControllerButton button);

	bool ButtonTriggerRelease(ControllerButton button);

	bool ButtonKeepRelease(ControllerButton button);

	Vector2 GetLeftStickVec(Vector2 deadRate = { 1.0f,1.0f });

	Vector2 GetRightStickVec(Vector2 deadRate = { 1.0f,1.0f });

	static Controller* GetInstance();

private:

	Controller() = default;
	~Controller() = default;

	Controller& operator=(const Controller&) = delete;
	Controller(const Controller&) = delete;
};