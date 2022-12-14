#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "WindowApi.h"

class DirectXCommon {
public:

	void Initialize();
	void InitializeDevice();
	void InitializeComand();
	void InitializeSwapchain();

private:	
	WindowApi* window = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
};