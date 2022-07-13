#pragma once
#include <d3d12.h>

class Port
{
public:
	//ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};

	//シザー
	D3D12_RECT scissorRec{};

	//ポート範囲
	void DrawViewPort(ID3D12GraphicsCommandList* commandList);
	//シザー範囲
	void DrawScissor(ID3D12GraphicsCommandList* commandList);

	//初期化(固定値)
	Port();

	//初期化(代入)
	Port(int Width,int Height);

	//描画範囲の変更
	void ChengePort(int Width, int Height);
};