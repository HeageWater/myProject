#pragma once
#include <Windows.h>

class WindowApi
{
public:

	void Initialize();

	void Finalize();

	bool ProcessMessege();

	WNDCLASSEX w{};

	//ウィンドウサイズ
	static const int window_width = 1280;
	static const int window_height = 720;

	//ウィンドウサイズ{x,y,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };

	HWND hwnd = nullptr;

	HWND GetHwnd() const { return hwnd; };

	HINSTANCE GetHInstance() const { return w.hInstance; };

	static WindowApi* Get() 
	{
		static WindowApi window;
		return &window;
	};
private:
	//インスタンス
	WindowApi();
	~WindowApi() {};


};