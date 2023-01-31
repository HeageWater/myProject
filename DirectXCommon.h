#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "WindowApi.h"
#include "viewport.h"

class DirectXCommon {
public:

	void Initialize(WindowApi* window);
	void InitializeDevice();
	void InitializeComand();
	void InitializeSwapchain();
	void InitializeREnderTargetView();
	void InitializeDeathBuffer();
	void InitializeFence();

	void PreDraw();
	void PostDraw();

	Port* port = new Port();
private:
	WindowApi* window = new WindowApi();

	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	//std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	//std::vector<ID3D12Resource*> backBuffers;

	HRESULT result;

	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;

	ID3D12DescriptorHeap* rtvHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	ID3D12DescriptorHeap* dsvHeap = nullptr;
	D3D12_RESOURCE_BARRIER barrierDesc{};

	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;



	/*HRESULT result;

	Microsoft::WRL::ComPtr<IDXGISwapChain4*> swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator*> cmdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList*> commandList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue*> commandQueue = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap*> rtvHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap*> dsvHeap = nullptr;
	D3D12_RESOURCE_BARRIER barrierDesc{};

	Microsoft::WRL::ComPtr <ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;*/
};