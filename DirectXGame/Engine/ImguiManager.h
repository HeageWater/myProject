#pragma once
#include "DirectX.h"

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

	//static ImguiManager* GetInstance();

private:

	//ImguiManager() = default;
	//~ImguiManager() = default;

	////�R�s�[�R���X�g���N�^�E������Z�q�폜
	//ImguiManager& operator=(const ImguiManager&) = delete;
	//ImguiManager(const ImguiManager&) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};