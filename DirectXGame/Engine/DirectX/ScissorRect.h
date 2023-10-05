#pragma once
#include <d3d12.h>

class ScissorRect
{
private:
	D3D12_RECT scissorRect{};
public:
	ScissorRect();
	void Init(const float left, const float width, const float top, const float height);
	void Update(ID3D12GraphicsCommandList* cmdList);
};

