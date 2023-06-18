#include "ViewPort.h"

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

	mat.m_[0][0] = viewport.Width / 2;
	mat.m_[1][1] = -viewport.Height / 2;
	mat.m_[3][0] = viewport.Width / 2;
	mat.m_[3][1] = viewport.Height / 2;
}

void ViewPort::Update(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->RSSetViewports(1, &viewport);
}
