#include "key.h"

Key::Key(WNDCLASSEX a, HWND hw)
{
	result = DirectInput8Create(
		a.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
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
	delete keyboard;
	delete directInput;
}

//更新
void Key::Update()
{
	//キーボード情報の取得開始
	keyboard->Acquire();

	//全キーの入力状態を保存する
	for (int i = 0; i < sizeof(oldkey); i++)
	{
		oldkey[i] = key[i];
	}

	keyboard->GetDeviceState(sizeof(key), key);

}

//押した瞬間
bool Key::PushKey(bool key)
{
	if (key == true)
	{
		return true;
	}
	return false;
}