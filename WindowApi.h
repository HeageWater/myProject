#pragma once
#include <Windows.h>

class WindowApi
{
public:
	//インスタンス
	WindowApi();

	void Initialize();

	WNDCLASSEX w{};

	//ウィンドウサイズ
	static const int window_width = 1280;
	static const int window_height = 720;
	 
	//ウィンドウサイズ{x,y,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };

	HWND hwnd = nullptr;
};