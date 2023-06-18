#include "viewport.h"

//初期化(固定値)
Port::Port()
{
	//ビュー設定
	viewport.Width = 1280;
	viewport.Height = 720;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//シザー設定
	scissorRec.left = 0;				 //切り抜き座標左
	scissorRec.right = 1280;	 //切り抜き座標右
	scissorRec.top = 0;				 //切り抜き座標上
	scissorRec.bottom = 720;	 //切り抜き座標下
}

//初期化(代入)
Port::Port(size_t Width, size_t Height)
{
	//ビュー設定
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//シザー設定
	scissorRec.left = 0;				 //切り抜き座標左
	scissorRec.right = Width;	 //切り抜き座標右
	scissorRec.top = 0;				 //切り抜き座標上
	scissorRec.bottom = Height;	 //切り抜き座標下
}

//描画範囲の変更
void Port::ChengePort(size_t Width, size_t Height)
{
	viewport.Width = Width;
	viewport.Height = Height;
}

//ポート範囲
void Port::DrawViewPort(ID3D12GraphicsCommandList* commandList)
{
	//ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1, &viewport);
}

//シザー範囲
void Port::DrawScissor(ID3D12GraphicsCommandList* commandList)
{
	//シザー矩形設定コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1, &scissorRec);
}

