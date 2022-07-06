#pragma once
#include <d3d12.h>

class Port
{
public:
	//ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};

	//シザー
	D3D12_RECT scissorRec{};

	void DrawViewPort(ID3D12GraphicsCommandList* commandList);
	void DrawScissor(ID3D12GraphicsCommandList* commandList);

	Port();
	Port(int Width,int Height);
};