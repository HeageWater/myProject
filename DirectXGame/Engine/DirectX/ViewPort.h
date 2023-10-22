#pragma once
#include "Matrix.h"
#include <d3d12.h>
#include <d3dcompiler.h>

/// <summary>
/// 描画範囲
/// </summary>
class ViewPort
{

public:

	ViewPort();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="topLftX"></param>
	/// <param name="topLftY"></param>
	/// <param name="MinDepth"></param>
	/// <param name="MaxDepth"></param>
	void Init(const float width, const float height, const float topLftX, const float topLftY, const float MinDepth = 0.0f, const float MaxDepth = 1.0f);
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cmdList"></param>
	void Update(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// matrixを返す
	/// </summary>
	/// <returns></returns>
	Matrix Mat() { return mat_; }

private:

	//viewport
	D3D12_VIEWPORT viewport_{};

	//matrix
	Matrix mat_;

};

