#pragma once
#include "Re///DirectX.h"

class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();

	//������(DirectXCommon�Ɉˑ�)
	void Initialize(MyDirectX* dxCommon);

	//���
	void Finalize();

	//�J�n
	void Begin();

	//�I��
	void End();

	//�`��
	void Draw(MyDirectX* dxCommon);

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};