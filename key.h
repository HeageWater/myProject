#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Key
{
private:

	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard = nullptr;

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;

	HRESULT result;

	BYTE key[256] = {};

	//全キーの入力状態を保存する
	BYTE oldkey[256] = {};

public:
	//インスタンス
	Key();
	Key(WNDCLASSEX a, HWND hw);
	~Key();
	//更新
	void Update();
	//押した時
	bool PushKey(bool key);
	//押してる間
	bool KeepPushKey(bool key);
	//離した瞬間
	bool ReleaseKey(bool key);
};