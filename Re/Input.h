#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include "Vector2D.h"
#include "Window.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input
{
public:
	static enum MouseButton {
		LeftClick = 0,
		RightClick,
		WheelClick,
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
	Input(Window* win_);
	~Input();
	void Initialize(Window* win_);
	void Update();

	bool GetKey(int _key);
	bool GetTrigger(int _key);
	bool ReleaseKey(int _key);

	bool Click(int type);
	bool ClickTrriger(int type);

	POINT CursorPos();
	void CursorPos(Vector2D& pos);
	LONG Wheel();
};

