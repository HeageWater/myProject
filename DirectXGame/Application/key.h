#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include <DirectXMath.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Key
{
private:

	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard = nullptr;

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;

public:

	HRESULT result;

	BYTE keys[256] = {};

	//全キーの入力状態を保存する
	BYTE oldkey[256] = {};

	//インスタンス
	Key();
	Key(WNDCLASSEX a, HWND hw);
	~Key();

	//更新
	void Update();
	//押した時
	bool Push(uint8_t key);
	//押してる間
	bool Keep(uint8_t key);
	//離した瞬間
	bool Release(uint8_t key);
};