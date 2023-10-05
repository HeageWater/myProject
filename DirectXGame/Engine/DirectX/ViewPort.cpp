#include "ViewPort.h"
#pragma comment(lib, "d3dcompiler.lib")

ViewPort::ViewPort()
{
}

void ViewPort::Init(const float width, const float height, const float topLftX, const float topLftY, const float MinDepth, const float MaxDepth)
{
	viewport.Width = width;
	viewport.Height = height;
	viewport.TopLeftX = topLftX;
	viewport.TopLeftY = topLftY;
	viewport.MinDepth = MinDepth;
	viewport.MaxDepth = MaxDepth;

	mat.m[0][0] = viewport.Width / 2;
	mat.m[1][1] = -viewport.Height / 2;
	mat.m[3][0] = viewport.Width / 2;
	mat.m[3][1] = viewport.Height / 2;
}

void ViewPort::Update(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->RSSetViewports(1, &viewport);
}
