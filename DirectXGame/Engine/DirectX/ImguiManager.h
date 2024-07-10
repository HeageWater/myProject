#pragma once
#include "DirectX.h"
#include <imgui.h>

/// <summary>
/// ImGuiを使うためのクラス
/// </summary>
class ImguiManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Window* window);

	/// <summary>
	/// 破棄
	/// </summary>
	void Finalize();

	/// <summary>
	/// 開始
	/// </summary>
	void Begin();

	/// <summary>
	///	終了
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static ImguiManager* GetInstance();

private:

	ImguiManager() = default;
	~ImguiManager() = default;

	//コピーコンストラクタ・代入演算子削除
	ImguiManager& operator=(const ImguiManager&) = delete;
	ImguiManager(const ImguiManager&) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};