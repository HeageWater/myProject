#pragma once
#include "DirectXCommon.h"

class ImguiManager
{
public:
	ImguiManager();
	~ImguiManager();

	//������(DirectXCommon�Ɉˑ�)
	void Initialize(DirectXCommon* dxCommon);

	//���
	void Finalize();

	//�J�n
	void Begin();

	//�I��
	void End();

	//�`��
	void Draw(DirectXCommon* dxCommon);

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};