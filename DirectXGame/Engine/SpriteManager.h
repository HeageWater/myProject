//#pragma once
//#include "DirectX.h"
//#include "VertBuff.h"
//#include <DirectXMath.h>
//#include <d3dcompiler.h>
//#include <vector>
//#include <fstream>
//
//class SpriteCommon
//{
//public:
//	SpriteCommon();
//	~SpriteCommon();
//	void Inilialize(MyDirectX* dxCommon);
//	void Draw();
//	MyDirectX* dxCommon_;
//
//	HRESULT result;
//
//	//�p�C�v�����X�e�[�g�̐���
//	//Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState{};
//	//���[�g�V�O�l�`��
//	//Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature{};
//	// ���_�o�b�t�@�r���[�̍쐬
//	//Microsoft::WRL::ComPtr<D3D12_VERTEX_BUFFER_VIEW> vbView{};
//
//	//�p�C�v�����X�e�[�g�̐���
//	ID3D12PipelineState* pipelineState = nullptr;
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
//	//���[�g�V�O�l�`��
//	ID3D12RootSignature* rootSignature;
//	// ���_�o�b�t�@�r���[�̍쐬
//	D3D12_VERTEX_BUFFER_VIEW vbView{};
//
//	ID3D12GraphicsCommandList* GetCommandList() const { return dxCommon_->GetCmdList(); };
//
//	UINT verticesCount;
//};
