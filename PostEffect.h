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

	//パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//ID3D12GraphicsCommandList* GetCommandList() const { return dxCommon_->GetCommandList(); };

	UINT verticesCount;

	//定数バッファ
	ID3D12Resource* constBuffTransform;
	//定数バッファマップ(行列用)
	ConstBufferDataTransform* constMapTransform;

public:
	//アフィン変換情報

	Sprite* sprite = new Sprite();

	//アフィン変換情報
	XMFLOAT3 scale = { 30,30,30 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 10,10,10 };
	//ワールド変換行列
	XMMATRIX matWorld;
protected:
	//頂点データ
	Vertex vertices[4] =
	{
		//	x,		y,		z,		u,	 v
		//前
		{{ 5.0f,   5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ 5.0f,  10.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{ 10.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{ 10.0f, 10.0f, -5.0f},{},{1.0f,0.0f}},//右上 
	};

	//Vector3 vertices[] =
	//{
	//	{-1.5f,-1.5f,0.0f},
	//	{-1.5f,+1.5f,0.0f},
	//	{+1.5f,-1.5f,0.0f}
	//};

	//Vector3 vertices[4] =
	//{
	//	{+2.5f,+2.5f,0.0f},//左下
	//	{+4.5f,+2.5f,0.0f},//右下
	//	{+2.5f,+4.5f,0.0f},//左上
	//	{+4.5f,+4.5f,0.0f},//右上
	//};

	uint16_t indices[6] =
	{
		0,1,2,
		1,2,3,
	};

	HRESULT result;
};