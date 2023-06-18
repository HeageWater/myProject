#pragma once
#include <d3d12.h>

class ScissorRect
{
private:
	D3D12_RECT scissorRect{};
public:
	ScissorRect();
	void Init(const size_t left, const size_t width, const size_t top, const size_t height);
	void Update(ID3D12GraphicsCommandList* cmdList);
};

