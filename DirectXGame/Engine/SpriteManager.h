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
//	//パイプランステートの生成
//	//Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState{};
//	//ルートシグネチャ
//	//Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature{};
//	// 頂点バッファビューの作成
//	//Microsoft::WRL::ComPtr<D3D12_VERTEX_BUFFER_VIEW> vbView{};
//
//	//パイプランステートの生成
//	ID3D12PipelineState* pipelineState = nullptr;
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
//	//ルートシグネチャ
//	ID3D12RootSignature* rootSignature;
//	// 頂点バッファビューの作成
//	D3D12_VERTEX_BUFFER_VIEW vbView{};
//
//	ID3D12GraphicsCommandList* GetCommandList() const { return dxCommon_->GetCmdList(); };
//
//	UINT verticesCount;
//};
