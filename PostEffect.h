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
	XMFLOAT3 scale = { 10,10,10 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//ワールド変換行列
	XMMATRIX matWorld;

	Sprite* sprite = new Sprite();
};