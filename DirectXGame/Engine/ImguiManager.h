#pragma once
#include "DirectX.h"

class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();

	//初期化(DirectXCommonに依存)
	void Initialize(MyDirectX* dxCommon);

	//解放
	void Finalize();

	//開始
	void Begin();

	//終了
	void End();

	//描画
	void Draw(MyDirectX* dxCommon);

	//static ImguiManager* GetInstance();

private:

	//ImguiManager() = default;
	//~ImguiManager() = default;

	////コピーコンストラクタ・代入演算子削除
	//ImguiManager& operator=(const ImguiManager&) = delete;
	//ImguiManager(const ImguiManager&) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};