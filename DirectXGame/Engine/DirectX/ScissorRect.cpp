#include "ScissorRect.h"

ScissorRect::ScissorRect()
{
}

void ScissorRect::Init(const float left, const float width, const float top, const float height)
{
	scissorRect_.left = (long)left; // 切り抜き座標左
	scissorRect_.right = scissorRect_.left + (long)width; // 切り抜き座標右
	scissorRect_.top = (long)top; // 切り抜き座標上
	scissorRect_.bottom = scissorRect_.top + (long)height; // 切り抜き座標下
}

void ScissorRect::Update(ID3D12GraphicsCommandList* cmdList)
{
	// シザー矩形設定コマンドを、コマンドリストに積む
	cmdList->RSSetScissorRects(1, &scissorRect_);
}
