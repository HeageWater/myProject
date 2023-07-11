#pragma once
#include "DirectXCommon.h"

class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();

	//‰Šú‰»(DirectXCommon‚ÉˆË‘¶)
	void Initialize(DirectXCommon* dxCommon);

	//‰ğ•ú
	void Finalize();

	//ŠJn
	void Begin();

	//I—¹
	void End();

	//•`‰æ
	void Draw(DirectXCommon* dxCommon);

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};