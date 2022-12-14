#include "DirectXCommon.h"

#include <vector>
#include <cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

void DirectXCommon::Initialize()
{
	assert(window);

	this->window = window;
}

void DirectXCommon::InitializeDevice()
{
	HRESULT result;

	ComPtr<IDXGIFactory7> dxgiFactory;

	//DXGIファクトリーの作成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeComand()
{
}

void DirectXCommon::InitializeSwapchain()
{
}
