#pragma once
#include <Windows.h>

class Window
{
public:
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280; // ����
	static const int window_height = 720; // �c��
private:
	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	// �E�B���h�E�I�u�W�F�N�g�̐���
	HWND hwnd;
	// ���b�Z�[�W
	MSG msg{};
	// �E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//Window() = default;
	//~Window() = default;

	////�R�s�[�R���X�g���N�^�E������Z�q�폜
	//Window& operator=(const Window&) = delete;
	//Window(const Window&) = delete;
public:

	Window();
	~Window();
	void Initialize();
	void MsgUpdate();
	bool EndLoop();

	//	getter
	HWND GetHwnd() const { return hwnd; }
	WNDCLASSEX GetWND() const { return w; }

	/*Window* GetInstance()
	{
		static Window window_;
		return &window_;
	}*/
};

