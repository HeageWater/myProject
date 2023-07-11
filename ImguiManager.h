#pragma once
#include "Re///DirectX.h"

class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();

	//‰Šú‰»(DirectXCommon‚ÉˆË‘¶)
	void Initialize(MyDirectX* dxCommon);

	//‰ğ•ú
	void Finalize();

	//ŠJn
	void Begin();

	//I—¹
	void End();

	//•`‰æ
	void Draw(MyDirectX* dxCommon);

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};