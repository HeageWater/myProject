#pragma once
#include <Windows.h>

class WindowApi
{
public:

	void Initialize();

	void Finalize();

	bool ProcessMessege();

	WNDCLASSEX w{};

	//�E�B���h�E�T�C�Y
	static const int window_width = 1280;
	static const int window_height = 720;

	//�E�B���h�E�T�C�Y{x,y,����,�c��}
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
	//�C���X�^���X
	WindowApi();
	~WindowApi() {};


};