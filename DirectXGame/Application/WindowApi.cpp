#include "WindowApi.h"

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WindowApi::WindowApi()
{
	//�E�B���h�E�N���X�̐ݒ�
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = L"DirectXGame";
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	//�E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);

	//�����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	 hwnd = CreateWindow(w.lpszClassName, //�N���X��
		L"DirectXGame",						  //�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,				  //�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,						  //�\��x���W(OS�ɔC����)
		CW_USEDEFAULT,						  //�\��y���W(OS�ɔC����)
		wrc.right - wrc.left,				  //�E�B���h�E����
		wrc.bottom - wrc.top,				  //�E�B���h�E�c��
		nullptr,							  //�e�E�B���h�E�n���h��
		nullptr,							  //���j���[�n���h��
		w.hInstance,						  //�Ăяo���A�v���P�[�V����
		nullptr);							  //�I�v�V����

	//�E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);
}