#pragma once
#include "Sprite.h"
#include "Vector2.h"
#include "object.h"
using namespace Microsoft::WRL;

class  PostEffect : Sprite
{
public:
	PostEffect(ID3D12Device* device);
	~PostEffect();

	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	void PostDraw(ID3D12GraphicsCommandList* cmdList);

	void Draw(ID3D12GraphicsCommandList* cmdList, ID3D12PipelineState* pipelineState, ID3D12RootSignature* rootSignature);
	void Update();
	void Initialize();

	void CreateGraphicsPipelineState();

	ComPtr<ID3D12PipelineState> pipelineStateP;
	ComPtr<ID3D12RootSignature> rootSignatureP;

	static const float clearColor[4];

	//ConstBufferDataMaterial materialPos;
	ConstBufferDataTransform transform;
	Vector2 position;
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
};