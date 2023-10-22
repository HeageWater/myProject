#include "Input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

void Input::Initialize(Window* win)
{
	win_ = win;
	inputHwnd_ = win_->GetHwnd();
	//	DirectInput初期化
	HRESULT result = DirectInput8Create(
		win_->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));
	//	デバイス生成(キーボード以外も可能)
	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = keyboard_->SetCooperativeLevel(
		win_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));


	result = DirectInput8Create(
		win_->GetWND().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));
	//	デバイス生成(キーボード以外も可能)
	result = directInput_->CreateDevice(GUID_SysMouse, &mouse_, NULL);
	assert(SUCCEEDED(result));
	//	入力形成のセット
	result = mouse_->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//	排他制御のレベルセット
	result = mouse_->SetCooperativeLevel(
		win_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	//	前フレームの情報取得
	for (size_t i = 0; i < sizeof(key_); i++)
	{
		prev_[i] = key_[i];
	}

	prevclick_ = click_;

	//	キー情報取得
	keyboard_->Acquire();
	//	全キーの入力情報取得
	keyboard_->GetDeviceState(sizeof(key_), key_);

	mouse_->Acquire();
	mouse_->Poll();
	mouse_->GetDeviceState(sizeof(DIMOUSESTATE), &click_);
	GetCursorPos(&cursor_);
	ScreenToClient(win_->GetHwnd(), &cursor_);
}

void Input::Finalize()
{
	keyboard_->Unacquire();
	keyboard_->Release();
	mouse_->Unacquire();
	mouse_->Release();
	directInput_->Release();
}

bool Input::GetKey(size_t _key)
{
	return key_[_key];
}

bool Input::GetTrigger(size_t _key)
{
	return key_[_key] && !prev_[_key];
}

bool Input::ReleaseKey(size_t _key)
{
	return prev_[_key] && !key_[_key];
}

bool Input::Click(size_t type)
{
	return (click_.rgbButtons[type] & (0x80));

}

bool Input::ClickTrriger(size_t type)
{
	return (click_.rgbButtons[type] & (0x80)) && !(prevclick_.rgbButtons[type] & (0x80));
}

POINT Input::CursorPos()
{
	ScreenToClient(inputHwnd_, &cursor_);
	return cursor_;
}

void Input::CursorPos(Vector2D& pos)
{
	pos.x_ = (float)cursor_.x;
	pos.y_ = (float)cursor_.y;
}

LONG Input::Wheel()
{
	return click_.lZ;
}

Input* Input::GetInstance()
{
	static Input input;
	return &input;
}
