#include "Input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input::Input(Window* win_)
{
	Initialize(win_);
}

Input::~Input()
{
	keyboard->Unacquire();
	keyboard->Release();
	mouse->Unacquire();
	mouse->Release();
	directInput->Release();
}

void Input::Initialize(Window* win_)
{
	win = win_;
	inputHwnd = win->GetHwnd();
	//	DirectInput初期化
	HRESULT result = DirectInput8Create(
		win->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//	デバイス生成(キーボード以外も可能)
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = keyboard->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));


	result = DirectInput8Create(
		win->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//	デバイス生成(キーボード以外も可能)
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = mouse->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//	前フレームの情報取得
	for (size_t i = 0; i < sizeof(key); i++)
	{
		prev[i] = key[i];
	}

	prevclick = click;

	//	キー情報取得
	keyboard->Acquire();
	//	全キーの入力情報取得
	keyboard->GetDeviceState(sizeof(key), key);

	mouse->Acquire();
	mouse->Poll();
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &click);
	GetCursorPos(&cursor);
	ScreenToClient(win->GetHwnd(), &cursor);
}

bool Input::GetKey(size_t _key)
{
	return key[_key];
}

bool Input::GetTrigger(size_t _key)
{
	return key[_key] && !prev[_key];
}

bool Input::ReleaseKey(size_t _key)
{
	return prev[_key] && !key[_key];
}

bool Input::Click(size_t type)
{
	return (click.rgbButtons[type] & (0x80));

}

bool Input::ClickTrriger(size_t type)
{
	return (click.rgbButtons[type] & (0x80)) && !(prevclick.rgbButtons[type] & (0x80));
}

POINT Input::CursorPos()
{
	ScreenToClient(inputHwnd, &cursor);
	return cursor;
}

void Input::CursorPos(Vector2D& pos)
{
	pos.x = (float)cursor.x;
	pos.y = (float)cursor.y;
}

LONG Input::Wheel()
{
	return click.lZ;
}
