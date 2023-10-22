#include "Window.h"
#pragma comment(lib,"winmm.lib")
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPram);

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{ {}
if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
{
	return true;
}
switch (msg) {
case WM_DESTROY:
	PostQuitMessage(0);
	return 0;
}

return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::Window()
{
	Initialize();
}

Window::~Window()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}

void Window::Initialize()
{
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc;
	w_.lpszClassName = L"Blade";
	w_.hInstance = GetModuleHandle(nullptr);
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w_);

	RECT wrc = { 0, 0, window_width_, window_height_ };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(w_.lpszClassName,
		L"Blade",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w_.hInstance,
		nullptr);

	ShowWindow(hwnd_, SW_SHOW);

	timeBeginPeriod(1);
}

void Window::MsgUpdate()
{
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_);
		DispatchMessage(&msg_);
	}
}

bool Window::EndLoop()
{
	if (msg_.message == WM_QUIT)
	{
		return true;
	}
	return false;
}
