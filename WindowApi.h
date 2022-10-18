#pragma once
#include <Windows.h>

//ウィンドウサイズ
static const int window_width = 1280;
static const int window_height = 720;

class WindowApi
{
public:
	//インスタンス
	WindowApi();
	~WindowApi();

	WNDCLASSEX w{};
	MSG msg{};

	//ウィンドウサイズ{x,y,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };

	HWND hwnd = nullptr;

	//ループ抜ける
	bool breakLoop();

	//ウィンドウメッセージ処理
	void MsgMessege();
};