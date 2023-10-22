#pragma once
#include <d3d12.h>

/// <summary>
/// シザースクラス(描画しない場所を設定する)
/// </summary>
class ScissorRect
{
private:
	D3D12_RECT scissorRect_{};
public:
	ScissorRect();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="left"></param>
	/// <param name="width"></param>
	/// <param name="top"></param>
	/// <param name="height"></param>
	void Init(const float left, const float width, const float top, const float height);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cmdList"></param>
	void Update(ID3D12GraphicsCommandList* cmdList);
};

