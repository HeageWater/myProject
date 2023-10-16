#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include "Vector2D.h"
#include "Window.h"

/// <summary>
/// キーボードの入力をとる
/// </summary>
class Input
{
public:
	enum MouseButton {
		LeftClick = 0,
		RightClick = 1,
		WheelClick = 2,
	};
private:
	Window* win = nullptr;
	HWND inputHwnd;

	BYTE key[256] = {};
	BYTE prev[256] = {};
	IDirectInputDevice8* keyboard = nullptr;
	IDirectInput8* directInput = nullptr;

	DIMOUSESTATE click = {};
	DIMOUSESTATE prevclick = {};
	IDirectInputDevice8* mouse = nullptr;
	POINT cursor;
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="win_">windowクラスを初期化に一回だけ呼ぶ</param>
	void Initialize(Window* win_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// キーを押しているか
	/// </summary>
	/// <param name="_key"></param>
	/// <returns></returns>
	bool GetKey(size_t _key);

	/// <summary>
	/// キーを押した瞬間
	/// </summary>
	/// <param name="_key"></param>
	/// <returns></returns>
	bool GetTrigger(size_t _key);

	/// <summary>
	/// キーを離した瞬間
	/// </summary>
	/// <param name="_key"></param>
	/// <returns></returns>
	bool ReleaseKey(size_t _key);

	/// <summary>
	/// マウスをクリックしているか
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool Click(size_t type);

	/// <summary>
	/// マウスを押した瞬間
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool ClickTrriger(size_t type);

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns></returns>
	POINT CursorPos();

	/// <summary>
	/// マウスの位置(vector2)
	/// </summary>
	/// <param name="pos"></param>
	void CursorPos(Vector2D& pos);

	/// <summary>
	/// ホイール
	/// </summary>
	/// <returns></returns>
	LONG Wheel();

	//シングルトン
	static Input* GetInstance();

private:

	Input() = default;
	~Input() = default;

	//コピーコンストラクタ・代入演算子削除
	Input& operator=(const Input&) = delete;
	Input(const Input&) = delete;
};

