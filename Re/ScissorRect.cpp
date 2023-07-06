#include "ScissorRect.h"

ScissorRect::ScissorRect()
{
}

void ScissorRect::Init(const size_t left, const size_t width, const size_t top, const size_t height)
{
	scissorRect.left = (long)left; // 切り抜き座標左
	scissorRect.right = (long)(scissorRect.left + width); // 切り抜き座標右
	scissorRect.top = (long)top; // 切り抜き座標上
	scissorRect.bottom = (long)(scissorRect.top + height); // 切り抜き座標下
}

void ScissorRect::Update(ID3D12GraphicsCommandList* cmdList)
{
	// シザー矩形設定コマンドを、コマンドリストに積む
	cmdList->RSSetScissorRects(1, &scissorRect);
}
