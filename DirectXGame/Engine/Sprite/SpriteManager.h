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
	/*static SpriteCommon* Get()
	{
		static SpriteCommon spriteCommon_;
		return &spriteCommon_;
	}*/

	SpriteCommon();
	~SpriteCommon();

	void Inilialize(MyDirectX* dxCommon, bool isSemiArpha);
	void Draw();

	ID3D12GraphicsCommandList* GetCommandList() const { return dxCommon_->GetCmdList(); };
	ID3D12PipelineState* GetPipleLine() const { return pipelineState; };
	MyDirectX* GetDxCommon() const { return dxCommon_; };

	//private:
	MyDirectX* dxCommon_;

	//パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	UINT verticesCount;

private:
	/*SpriteCommon();
	~SpriteCommon();*/


	HRESULT result;
};
