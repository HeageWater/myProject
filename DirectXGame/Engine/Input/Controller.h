#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include<DirectXMath.h>
#include<wrl.h>
#include "myMath.h"
#include <xinput.h>

/// <summary>
/// コントローラーのボタン
/// </summary>
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

/// <summary>
/// スティック
/// </summary>
enum ControllerStick
{
	L_UP, L_DOWN, L_LEFT, L_RIGHT,
	R_UP, R_DOWN, R_LEFT, R_RIGHT, XBOX_STICK_NUM
};

/// <summary>
/// コントローラークラス
/// </summary>
class Controller
{
private:

	Microsoft::WRL::ComPtr<IDirectInputDevice8> controller_ = nullptr;
	XINPUT_STATE controllerState_{};
	XINPUT_STATE oldControllerState_{};
	//コントローラー振動強さ
	float shakePower_ = 0.0f;
	//コントローラー振動長さ(フレーム数)
	float shakeTimer_ = 0;

	//デッドゾーンに入っているか(DeadRate : デッドゾーン判定の度合い、1.0fだとデフォルト)
	bool StickInDeadZone(Vector2D thumb, const Vector2D deadRate);

	//最高入力強度
	const float STICK_INPUT_MAX = 32768.0f;

public:

	//更新処理(普段使わない)
	void Update();

	/// <summary>
	/// buttonで指定したボタンを押した瞬間
	/// </summary>
	/// <param name="button">AやLB等</param>
	/// <returns></returns>
	bool ButtonTriggerPush(ControllerButton button);

	/// <summary>
	/// buttonで指定したボタンを押している間
	/// </summary>
	/// <param name="button">AやLB等</param>
	/// <returns></returns>
	bool ButtonKeepPush(ControllerButton button);

	/// <summary>
	/// buttonで指定したボタンを離した瞬間
	/// </summary>
	/// <param name="button">AやLB等</param>
	/// <returns></returns>
	bool ButtonTriggerRelease(ControllerButton button);

	/// <summary>
	/// buttonで指定したボタンを押していない間
	/// </summary>
	/// <param name="button">AやLB等</param>
	/// <returns></returns>
	bool ButtonKeepRelease(ControllerButton button);

	/// <summary>
	/// stickで指定した方向に倒した瞬間
	/// </summary>
	/// <param name="stick">左スティックならL_〇〇,右スティックならR_〇〇</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns></returns>
	bool StickTriggerPush(ControllerStick stick, const float& deadRange = 0.3f, Vector2D deadRate = { 1.0f,1.0f });

	/// <summary>
	/// stickで指定した方向に倒している間
	/// </summary>
	/// <param name="stick">左スティックならL_〇〇,右スティックならR_〇〇</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns></returns>
	bool StickKeepPush(ControllerStick stick, const float& deadRange = 0.3f, Vector2D deadRate = { 1.0f,1.0f });

	/// <summary>
	/// stickで指定した方向から戻した瞬間
	/// </summary>
	/// <param name="stick">左スティックならL_〇〇,右スティックならR_〇〇</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns></returns>
	bool StickTriggerRelease(ControllerStick stick, const float& deadRange = 0.3f, Vector2D deadRate = { 1.0f,1.0f });

	/// <summary>
	/// stickで指定した方向に倒していない間
	/// </summary>
	/// <param name="stick">左スティックならL_〇〇,右スティックならR_〇〇</param>
	/// <param name="deadRange">デッドゾーンの範囲初期値0.3f</param>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns></returns>
	bool StickKeepRelease(ControllerStick stick, const float& deadRange = 0.3f, Vector2D deadRate = { 1.0f,1.0f });

	/// <summary>
	/// ゲームパッドの振動
	/// </summary>
	/// <param name="power">振動の強さ0.0f～1.0f</param>
	/// <param name="span">振動の時間フレーム</param>
	void ShakeController(const float& power, const float& span);

	/// <summary>
	/// Lスティックの傾きの取得
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns></returns>
	Vector2D GetLeftStickVec(Vector2D deadRate = { 1.0f,1.0f });

	/// <summary>
	/// Rスティックの傾きの取得
	/// </summary>
	/// <param name="deadRate">デッドゾーン判定の度合い初期値1.0f</param>
	/// <returns></returns>
	Vector2D GetRightStickVec(Vector2D deadRate = { 1.0f,1.0f });

	//シングルトン
	static Controller* GetInstance();

private:

	Controller() = default;
	~Controller() = default;

	//コピーコンストラクタ・代入演算子削除
	Controller& operator=(const Controller&) = delete;
	Controller(const Controller&) = delete;
};