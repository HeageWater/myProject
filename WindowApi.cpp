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

void WindowApi::Initialize() {

	////�E�B���h�E�N���X�̐ݒ�
	//w.cbSize = sizeof(WNDCLASSEX);
	//w.lpfnWndProc = (WNDPROC)WindowProc;
	//w.lpszClassName = L"DirectXGame";
	//w.hInstance = GetModuleHandle(nullptr);
	//w.hCursor = LoadCursor(NULL, IDC_ARROW);

	////�E�B���h�E�N���X��OS�ɓo�^����
	//RegisterClassEx(&w);

	////�����ŃT�C�Y��␳����
	//AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

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

void WindowApi::Finalize()
{
	//�E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
}

bool WindowApi::ProcessMessege()
{
	MSG msg{};

	//�E�B���h�E���b�Z�[�W����
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//x�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}
