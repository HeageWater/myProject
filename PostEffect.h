#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include "object.h"
using namespace Microsoft::WRL;

class  PostEffect : Sprite
{
public:
	PostEffect(ID3D12Device* device);
	PostEffect(Sprite* sprite);
	~PostEffect();

	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	void PostDraw(ID3D12GraphicsCommandList* cmdList);

	void Draw(ID3D12GraphicsCommandList* cmdList, ID3D12PipelineState* pipelineState, ID3D12RootSignature* rootSignature, D3D12_INDEX_BUFFER_VIEW& ibView);
	void Draw();
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Initialize();

	void CreateGraphicsPipelineState();

	ComPtr<ID3D12PipelineState> pipelineStateP;
	ComPtr<ID3D12RootSignature> rootSignatureP;

	static const float clearColor[4];

private:
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	//DirectXCommon* dxCommon_;
	ID3D12Device* device;

	//�p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//ID3D12GraphicsCommandList* GetCommandList() const { return dxCommon_->GetCommandList(); };

	UINT verticesCount;

	//�萔�o�b�t�@
	ID3D12Resource* constBuffTransform;
	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform;

public:
	//�A�t�B���ϊ����

	Sprite* sprite = new Sprite();

	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 30,30,30 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 10,10,10 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
protected:
	//���_�f�[�^
	Vertex vertices[4] =
	{
		//	x,		y,		z,		u,	 v
		//�O
		{{ 5.0f,   5.0f, -5.0f},{},{0.0f,1.0f}},//����
		{{ 5.0f,  10.0f, -5.0f},{},{0.0f,0.0f}},//����
		{{ 10.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{ 10.0f, 10.0f, -5.0f},{},{1.0f,0.0f}},//�E�� 
	};

	//Vector3 vertices[] =
	//{
	//	{-1.5f,-1.5f,0.0f},
	//	{-1.5f,+1.5f,0.0f},
	//	{+1.5f,-1.5f,0.0f}
	//};

	//Vector3 vertices[4] =
	//{
	//	{+2.5f,+2.5f,0.0f},//����
	//	{+4.5f,+2.5f,0.0f},//�E��
	//	{+2.5f,+4.5f,0.0f},//����
	//	{+4.5f,+4.5f,0.0f},//�E��
	//};

	uint16_t indices[6] =
	{
		0,1,2,
		1,2,3,
	};

	HRESULT result;
};