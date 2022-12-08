#pragma once
#include "Window.h"
#include "ViewPort.h"
#include "ScissorRect.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <wrl.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class MyDirectX
{
private:
	Window* win = nullptr;

	HRESULT result;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device;

	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	ComPtr<ID3D12CommandQueue> cmdQueue;

	ComPtr<IDXGISwapChain4> swapChain;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// �o�b�N�o�b�t�@
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	D3D12_RESOURCE_BARRIER barrierDesc{};

	FLOAT clearColor[4] = {};

	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	ComPtr<ID3D12Resource> depthBuff;

	// screenTexture
	ComPtr<ID3D12Resource> screenResource;
	D3D12_RESOURCE_BARRIER screenBarrierDesc;
	ComPtr<ID3D12DescriptorHeap> screenRTVHeap;
	std::vector<ComPtr<ID3D12DescriptorHeap>> screenSRVHeap;
	
	//	�r���[�|�[�g
	ViewPort viewPort;
	// �V�U�[��`
	ScissorRect scissorRect;

	int textureNum;
	std::vector<ComPtr<ID3D12Resource>> texBuff;
	UINT incrementSize;

	std::chrono::steady_clock::time_point reference_;
private:
	void DebugLayer();

	void InitializeFPS();
	void UpdateFPS();

	void ScreenClear(FLOAT* clearColor, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);
	void ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);
	
	void SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource = nullptr);
	void CmdListDrawAble(D3D12_RESOURCE_BARRIER& barrierDesc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter,
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, FLOAT* clearColor = nullptr);
public:
	MyDirectX(Window* win_);
	void Initialize(Window* win_);
	void PrevDrawScreen(FLOAT* clearColor = nullptr);
	void PostDrawScreen();
	void PrevDraw(FLOAT* clearColor = nullptr);
	void PostDraw();

	int LoadTextureGraph(const wchar_t* textureName);

	//	Getter
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(int handle);
	ID3D12Device* GetDev() { return device.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
	Matrix GetViewportMat() { return viewPort.Mat(); }
	UINT GetIncrementSize() { return incrementSize; }
};

