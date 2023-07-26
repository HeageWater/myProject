#pragma once
#include "DirectX.h"
#include "VertBuff.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <fstream>

class SpriteCommon
{
public:
	static SpriteCommon* Get()
	{
		static SpriteCommon spriteCommon_;
		return &spriteCommon_;
	}
	void Inilialize(MyDirectX* dxCommon);
	void Draw();

	ID3D12GraphicsCommandList* GetCommandList() const { return dxCommon_->GetCmdList(); };
	ID3D12PipelineState* GetPipleLine() const { return pipelineState; };
	MyDirectX* GetDxCommon() const { return dxCommon_; };

	//private:
	MyDirectX* dxCommon_;

	//�p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	UINT verticesCount;

private:
	SpriteCommon();
	~SpriteCommon();


	HRESULT result;
};
