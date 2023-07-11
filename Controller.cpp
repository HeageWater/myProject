#include "Controller.h"

void Controller::Update()
{
	oldControllerState = controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	//コントローラー取得
	DWORD dwResult = XInputGetState(0, &controllerState);
}

bool Controller::ButtonTriggerPush(ControllerButton button)
{
	if (button == LT) {
		return oldControllerState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && ButtonKeepPush(button);
	}
	else if (button == RT) {
		return oldControllerState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && ButtonKeepPush(button);
	}
	else {
		return !(oldControllerState.Gamepad.wButtons & button) && ButtonKeepPush(button);
	}
}

bool Controller::ButtonKeepPush(ControllerButton button)
{
	if (button == LT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < controllerState.Gamepad.bLeftTrigger;
	}
	else if (button == RT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < controllerState.Gamepad.bRightTrigger;
	}
	else {
		return controllerState.Gamepad.wButtons & button;
	}
}

bool Controller::ButtonTriggerRelease(ControllerButton button)
{
	//トリガー
	if (button == LT) {
		return oldControllerState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && !ButtonKeepPush(button);
	}
	else if (button == RT) {
		return oldControllerState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && !ButtonKeepPush(button);
	}
	else {
		return oldControllerState.Gamepad.wButtons & button && !ButtonKeepPush(button);
	}
}

bool Controller::ButtonKeepRelease(ControllerButton button)
{
	if (button == LT) {
		return !(XINPUT_GAMEPAD_TRIGGER_THRESHOLD < controllerState.Gamepad.bLeftTrigger);
	}
	else if (button == RT) {
		return !(XINPUT_GAMEPAD_TRIGGER_THRESHOLD < controllerState.Gamepad.bRightTrigger);
	}
	else {
		return !(controllerState.Gamepad.wButtons & button);
	}
}

Vector2 Controller::GetLeftStickVec(Vector2 deadRate)
{
	Vector2 result(static_cast<float>(controllerState.Gamepad.sThumbLX), static_cast<float>(-controllerState.Gamepad.sThumbLY));

	result.x /= STICK_INPUT_MAX;
	result.y /= STICK_INPUT_MAX;

	return result;
}

Vector2 Controller::GetRightStickVec(Vector2 deadRate)
{
	Vector2 result(static_cast<float>(controllerState.Gamepad.sThumbRX), static_cast<float>(-controllerState.Gamepad.sThumbRY));

	result.x /= STICK_INPUT_MAX;
	result.y /= STICK_INPUT_MAX;

	return result;
}

Controller* Controller::GetInstance()
{
	static Controller instance;
	return &instance;
}