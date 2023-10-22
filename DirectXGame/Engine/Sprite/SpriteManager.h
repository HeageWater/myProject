#pragma once
#include "DirectX.h"
#include "VertBuff.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <fstream>

/// <summary>
/// スプライト管理用クラス
/// </summary>
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

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	/// <param name="isSemiArpha"></param>
	void Inilialize(MyDirectX* dxCommon, bool isSemiArpha);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList() const { return dxCommon_->GetCmdList(); };

	/// <summary>
	/// パイプライン取得
	/// </summary>
	/// <returns></returns>
	ID3D12PipelineState* GetPipleLine() const { return pipelineState_; };

	/// <summary>
	/// DirextX取得
	/// </summary>
	/// <returns></returns>
	MyDirectX* GetDxCommon() const { return dxCommon_; };

	//private:
	MyDirectX* dxCommon_;

	//パイプランステートの生成
	ID3D12PipelineState* pipelineState_ = nullptr;

	//パイプラインデスク
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};

	//ルートシグネチャ
	ID3D12RootSignature* rootSignature_;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//点の数
	UINT verticesCount_;

private:

	//結果
	HRESULT result_;
};
