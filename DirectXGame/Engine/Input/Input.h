#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include "Vector2D.h"
#include "Window.h"

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
	void Initialize(Window* win_);
	void Update();
	void Finalize();

	bool GetKey(size_t _key);
	bool GetTrigger(size_t _key);
	bool ReleaseKey(size_t _key);

	bool Click(size_t type);
	bool ClickTrriger(size_t type);

	POINT CursorPos();
	void CursorPos(Vector2D& pos);
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

