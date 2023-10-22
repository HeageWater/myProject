#include "ViewPort.h"
#pragma comment(lib, "d3dcompiler.lib")

ViewPort::ViewPort()
{
}

void ViewPort::Init(const float width, const float height, const float topLftX, const float topLftY, const float MinDepth, const float MaxDepth)
{
	viewport_.Width = width;
	viewport_.Height = height;
	viewport_.TopLeftX = topLftX;
	viewport_.TopLeftY = topLftY;
	viewport_.MinDepth = MinDepth;
	viewport_.MaxDepth = MaxDepth;

	mat_.m_[0][0] = viewport_.Width / 2;
	mat_.m_[1][1] = -viewport_.Height / 2;
	mat_.m_[3][0] = viewport_.Width / 2;
	mat_.m_[3][1] = viewport_.Height / 2;
}

void ViewPort::Update(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->RSSetViewports(1, &viewport_);
}
