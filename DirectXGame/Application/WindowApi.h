#pragma once
#include <Windows.h>

class WindowApi
{
public:
	//�C���X�^���X
	WindowApi();

	WNDCLASSEX w{};

	//�E�B���h�E�T�C�Y
	const int window_width = 1280;
	const int window_height = 720;

	//�E�B���h�E�T�C�Y{x,y,����,�c��}
	RECT wrc = { 0,0,window_width,window_height };

	HWND hwnd;
};