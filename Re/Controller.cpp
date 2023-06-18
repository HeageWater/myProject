#include "Controller.h"

bool Controller::StickInDeadZone(Vector2D thumb, const Vector2D deadRate)
{
	bool x = false;
	bool y = false;

	if ((thumb.x_ < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadRate.x_ && thumb.x_ > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadRate.x_)) {
		thumb.x_ = 0.0f;
		x = true;
	}
	if ((thumb.y_ < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadRate.y_ && thumb.y_ > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadRate.y_)) {
		thumb.y_ = 0.0f;
		y = true;
	}

	if (x && y) {
		return true;
	}

	return false;
}

void Controller::Update()
{
	oldControllerState = controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	//コントローラー取得
	DWORD dwResult = XInputGetState(0, &controllerState);

	if (dwResult == ERROR_SUCCESS) {
		//コントローラーが接続されている
		if (0 < shakeTimer) {
			shakeTimer--;
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			if (shakeTimer == 0) {
				vibration.wLeftMotorSpeed = static_cast<WORD>(0.0f); // use any value between 0-65535 here
				vibration.wRightMotorSpeed = static_cast<WORD>(0.0f); // use any value between 0-65535 here
			}
			else {
				vibration.wLeftMotorSpeed = static_cast<WORD>(65535.0f * shakePower); // use any value between 0-65535 here
				vibration.wRightMotorSpeed = static_cast<WORD>(65535.0f * shakePower); // use any value between 0-65535 here
			}

			XInputSetState(dwResult, &vibration);
		}
	}
	else {
		//コントローラーが接続されていない
	}
}

bool Controller::ButtonTriggerPush(ControllerButton button)
{
	//トリガー
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

bool Controller::StickTriggerPush(ControllerStick stick, const float& deadRange, const Vector2D deadRate)
{
	Vector2D oldVec;
	Vector2D vec;
	bool isLeftStick = stick <= L_RIGHT;
	if (isLeftStick) {
		oldVec = Vector2D(oldControllerState.Gamepad.sThumbLX, oldControllerState.Gamepad.sThumbLY);
		vec = Vector2D(controllerState.Gamepad.sThumbLX, controllerState.Gamepad.sThumbLY);
	}
	else {
		oldVec = Vector2D(oldControllerState.Gamepad.sThumbRX, oldControllerState.Gamepad.sThumbRY);
		vec = Vector2D(controllerState.Gamepad.sThumbRX, controllerState.Gamepad.sThumbRY);
	}

	if (!StickInDeadZone(oldVec, deadRate)) {
		return false;
	}

	if (StickInDeadZone(vec, deadRate)) {
		return false;
	}

	bool result = false;

	if (stick % 4 == L_UP) {
		result = !(deadRange < (oldVec.y_ / STICK_INPUT_MAX)) && deadRange < (vec.y_ / STICK_INPUT_MAX);
	}
	else if (stick % 4 == L_DOWN) {
		result = !(oldVec.y_ / STICK_INPUT_MAX < -deadRange) && vec.y_ / STICK_INPUT_MAX < -deadRange;
	}
	else if (stick % 4 == L_RIGHT) {
		result = !(deadRange < (oldVec.x_ / STICK_INPUT_MAX)) && deadRange < (vec.x_ / STICK_INPUT_MAX);
	}
	else if (stick % 4 == L_LEFT) {
		result = !(oldVec.x_ / STICK_INPUT_MAX < -deadRange) && vec.x_ / STICK_INPUT_MAX < -deadRange;
	}
	else {
		assert(0);
	}

	return result;
}

bool Controller::StickKeepPush(ControllerStick stick, const float& deadRange, Vector2D deadRate)
{
	Vector2D vec;
	bool isLeftStick = stick <= L_RIGHT;

	if (isLeftStick) {
		vec = Vector2D(controllerState.Gamepad.sThumbLX, controllerState.Gamepad.sThumbLY);
	}
	else {
		vec = Vector2D(controllerState.Gamepad.sThumbRX, controllerState.Gamepad.sThumbRY);
	}

	if (StickInDeadZone(vec, deadRate))return false;

	if (stick % 4 == L_UP) {
		return deadRange < (vec.y_ / STICK_INPUT_MAX);
	}
	else if (stick % 4 == L_DOWN) {
		return  vec.y_ / STICK_INPUT_MAX < -deadRange;
	}
	else if (stick % 4 == L_RIGHT) {
		return deadRange < (vec.x_ / STICK_INPUT_MAX);
	}
	else if (stick % 4 == L_LEFT) {
		return  vec.x_ / STICK_INPUT_MAX < -deadRange;
	}

	assert(0);
	return false;
}

bool Controller::StickTriggerRelease(ControllerStick stick, const float& deadRange, Vector2D deadRate)
{
	Vector2D oldVec;
	Vector2D vec;
	bool isLeftStick = stick <= L_RIGHT;

	if (isLeftStick) {
		oldVec = Vector2D(oldControllerState.Gamepad.sThumbLX, oldControllerState.Gamepad.sThumbLY);
		vec = Vector2D(controllerState.Gamepad.sThumbLX, controllerState.Gamepad.sThumbLY);
	}
	else {
		oldVec = Vector2D(oldControllerState.Gamepad.sThumbRX, oldControllerState.Gamepad.sThumbRY);
		vec = Vector2D(controllerState.Gamepad.sThumbRX, controllerState.Gamepad.sThumbRY);
	}

	if (!StickInDeadZone(oldVec, deadRate)) {
		return false;
	}

	if (StickInDeadZone(vec, deadRate)) {
		return false;
	}

	bool result = false;

	if (stick % 4 == L_UP) {
		result = deadRange < (oldVec.y_ / STICK_INPUT_MAX) && !(deadRange < (vec.y_ / STICK_INPUT_MAX));
	}
	else if (stick % 4 == L_DOWN) {
		result = oldVec.y_ / STICK_INPUT_MAX < -deadRange && !(vec.y_ / STICK_INPUT_MAX < -deadRange);
	}
	else if (stick % 4 == L_RIGHT) {
		result = deadRange < (oldVec.x_ / STICK_INPUT_MAX) && !(deadRange < (vec.x_ / STICK_INPUT_MAX));
	}
	else if (stick % 4 == L_LEFT) {
		result = oldVec.x_ / STICK_INPUT_MAX < -deadRange && !(vec.x_ / STICK_INPUT_MAX < -deadRange);
	}
	else {
		assert(0);
	}

	return result;
}

bool Controller::StickKeepRelease(ControllerStick stick, const float& deadRange, Vector2D deadRate)
{
	return !StickKeepPush(stick, deadRange, deadRate);
}

void Controller::ShakeController(const float& power, const size_t& span)
{
	if (!(0 < power && power <= 1.0f)) {
		assert(0);
	}

	shakePower = power;
	shakeTimer = span;
}

Vector2D Controller::GetLeftStickVec(Vector2D deadRate)
{
	Vector2D result(static_cast<float>(controllerState.Gamepad.sThumbLX), static_cast<float>(-controllerState.Gamepad.sThumbLY));
	StickInDeadZone(result, deadRate);
	result.x_ /= STICK_INPUT_MAX;
	result.y_ /= STICK_INPUT_MAX;

	return result;
}

Vector2D Controller::GetRightStickVec(Vector2D deadRate)
{
	Vector2D result(static_cast<float>(controllerState.Gamepad.sThumbRX), static_cast<float>(-controllerState.Gamepad.sThumbRY));
	StickInDeadZone(result, deadRate);
	result.x_ /= STICK_INPUT_MAX;
	result.y_ /= STICK_INPUT_MAX;

	return result;
}

Controller* Controller::GetInstance()
{
	static Controller instance;
	return &instance;
}