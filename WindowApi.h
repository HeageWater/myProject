#pragma once
#include <Windows.h>

class WindowApi
{
public:
	//インスタンス
	WindowApi();

	WNDCLASSEX w{};

	//ウィンドウサイズ
	const int window_width = 1280;
	const int window_height = 720;

	//ウィンドウサイズ{x,y,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };

	HWND hwnd;
};