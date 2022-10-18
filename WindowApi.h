#pragma once
#include <Windows.h>

//�E�B���h�E�T�C�Y
static const int window_width = 1280;
static const int window_height = 720;

class WindowApi
{
public:
	//�C���X�^���X
	WindowApi();
	~WindowApi();

	WNDCLASSEX w{};
	MSG msg{};

	//�E�B���h�E�T�C�Y{x,y,����,�c��}
	RECT wrc = { 0,0,window_width,window_height };

	HWND hwnd = nullptr;

	//���[�v������
	bool breakLoop();

	//�E�B���h�E���b�Z�[�W����
	void MsgMessege();
};