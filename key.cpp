#include "key.h"

Key::Key(HINSTANCE a, HWND hw)
{
	result = DirectInput8Create(
		a, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard,
		&keyboard, NULL);

	//入力データ形式のセット
	//標準形式
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		hw, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

Key::~Key()
{
}

//初期化
void Key::Initialize(WindowApi* winapi) {

	this->winapi = winapi;

	result = DirectInput8Create(
		winapi->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard,
		&keyboard, NULL);

	//入力データ形式のセット
	//標準形式
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		winapi->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

//更新
void Key::Update()
{
	//全キーの入力状態を保存する
	for (int i = 0; i < sizeof(oldkey); i++)
	{
		oldkey[i] = keys[i];
	}

	//キーボード情報の取得開始
	keyboard->Acquire();

	keyboard->GetDeviceState(sizeof(keys), keys);
}

//押した瞬間
bool Key::Push(uint8_t key)
{
	if (keys[key] != false &&
		oldkey[key] == false)
	{
		return true;
	}
	return false;
}

//押してる間
bool  Key::Keep(uint8_t key)
{
	if (keys[key] != false)
	{
		return true;
	}
	return false;
}

//離した瞬間
bool Key::Release(uint8_t key)
{
	if (keys[key] == false &&
		oldkey[key] != false)
	{
		return true;
	}
	return false;
}