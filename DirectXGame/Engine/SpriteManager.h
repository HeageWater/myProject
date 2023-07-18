#pragma once
#include "DirectX.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <fstream>
using namespace DirectX;

#pragma comment(lib,"d3dcompiler.lib")
//
////頂点データ構造体
//static struct Vertex
//{
//	XMFLOAT3 pos;
//	XMFLOAT3 noemal;
//	XMFLOAT2 uv;
//};

class SpriteCommon
{
public:
	SpriteCommon();
	~SpriteCommon();
	void Inilialize(MyDirectX* dxCommon);
	void Draw();
	MyDirectX* dxCommon_;

	HRESULT result;

	//パイプランステートの生成
	//Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState{};
	//ルートシグネチャ
	//Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature{};
	// 頂点バッファビューの作成
	//Microsoft::WRL::ComPtr<D3D12_VERTEX_BUFFER_VIEW> vbView{};

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
