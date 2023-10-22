#pragma once
#include <Windows.h>

/// <summary>
/// windowクラス
/// </summary>
class Window
{
public:

	// ウィンドウサイズ
	static const int window_width_ = 1280; // 横幅
	static const int window_height_ = 720; // 縦幅

private:

	// ウィンドウクラスの設定
	WNDCLASSEX w_{};

	// ウィンドウオブジェクトの生成
	HWND hwnd_;

	// メッセージ
	MSG msg_{};

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

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void MsgUpdate();

	/// <summary>
	/// 終了条件が達成しているか
	/// </summary>
	/// <returns></returns>
	bool EndLoop();

	//	getter

	/// <summary>
	/// Hwndを取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() const { return hwnd_; }

	/// <summary>
	/// Wを取得
	/// </summary>
	/// <returns></returns>
	WNDCLASSEX GetWND() const { return w_; }

};

