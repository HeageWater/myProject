#include "ScissorRect.h"

ScissorRect::ScissorRect()
{
}

void ScissorRect::Init(const float left, const float width, const float top, const float height)
{
	scissorRect.left = left; // 切り抜き座標左
	scissorRect.right = scissorRect.left + width; // 切り抜き座標右
	scissorRect.top = top; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + height; // 切り抜き座標下
}

void ScissorRect::Update(ID3D12GraphicsCommandList* cmdList)
{
	// シザー矩形設定コマンドを、コマンドリストに積む
	cmdList->RSSetScissorRects(1, &scissorRect);
}
