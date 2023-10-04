#pragma once
#include <Windows.h>

class Window
{
public:
	// ウィンドウサイズ
	static const int window_width = 1280; // 横幅
	static const int window_height = 720; // 縦幅
private:
	// ウィンドウクラスの設定
	WNDCLASSEX w{};
	// ウィンドウオブジェクトの生成
	HWND hwnd;
	// メッセージ
	MSG msg{};
	// ウィンドウプロシージャ
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//Window() = default;
	//~Window() = default;

	////コピーコンストラクタ・代入演算子削除
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

