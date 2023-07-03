#include "WindowApi.h"
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lPram);

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

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
	//ウィンドウクラスの設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = L"DirectXGame";
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(w.lpszClassName, //クラス名
		L"DirectXGame",						  //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,				  //標準的なウィンドウスタイル
		CW_USEDEFAULT,						  //表示x座標(OSに任せる)
		CW_USEDEFAULT,						  //表示y座標(OSに任せる)
		wrc.right - wrc.left,				  //ウィンドウ横幅
		wrc.bottom - wrc.top,				  //ウィンドウ縦幅
		nullptr,							  //親ウィンドウハンドル
		nullptr,							  //メニューハンドル
		w.hInstance,						  //呼び出しアプリケーション
		nullptr);							  //オプション

	//ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
}

void WindowApi::Initialize() {

	////ウィンドウクラスの設定
	//w.cbSize = sizeof(WNDCLASSEX);
	//w.lpfnWndProc = (WNDPROC)WindowProc;
	//w.lpszClassName = L"DirectXGame";
	//w.hInstance = GetModuleHandle(nullptr);
	//w.hCursor = LoadCursor(NULL, IDC_ARROW);

	////ウィンドウクラスをOSに登録する
	//RegisterClassEx(&w);

	////自動でサイズを補正する
	//AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(w.lpszClassName, //クラス名
		L"DirectXGame",						  //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,				  //標準的なウィンドウスタイル
		CW_USEDEFAULT,						  //表示x座標(OSに任せる)
		CW_USEDEFAULT,						  //表示y座標(OSに任せる)
		wrc.right - wrc.left,				  //ウィンドウ横幅
		wrc.bottom - wrc.top,				  //ウィンドウ縦幅
		nullptr,							  //親ウィンドウハンドル
		nullptr,							  //メニューハンドル
		w.hInstance,						  //呼び出しアプリケーション
		nullptr);							  //オプション

	//ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
}

void WindowApi::Finalize()
{
	//ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

bool WindowApi::ProcessMessege()
{
	MSG msg{};

	//ウィンドウメッセージ処理
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//xボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}
