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

/// <summary>
/// DrectXクラス(基盤クラス)
/// </summary>
class MyDirectX
{
private:
	Window* win = nullptr;

	HRESULT result_;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device;

	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	ComPtr<ID3D12CommandQueue> cmdQueue;

	ComPtr<IDXGISwapChain4> swapChain;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// バックバッファ
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

	//	ビューポート
	ViewPort viewPort;
	// シザー矩形
	ScissorRect scissorRect;

	int textureNum;
	std::vector<ComPtr<ID3D12Resource>> texBuff;
	UINT incrementSize;

	std::chrono::steady_clock::time_point reference_;
private:

	/// <summary>
	/// デバッグレイヤー
	/// </summary>
	void DebugLayer();

	/// <summary>
	/// FPS初期化
	/// </summary>
	void InitializeFPS();

	//FPS更新
	void UpdateFPS();

	/// <summary>
	/// スクリーンクリア(色指定)
	/// </summary>
	/// <param name="clearColor">色指定</param>
	/// <param name="rtvHandle"></param>
	void ScreenClear(FLOAT* clearColor, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);

	/// <summary>
	/// スクリーンクリア
	/// </summary>
	/// <param name="rtvHandle"></param>
	void ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle);

	/// <summary>
	/// セットリソースバリア
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="StateBefore"></param>
	/// <param name="StateAfter"></param>
	/// <param name="pResource"></param>
	void SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource = nullptr);
	
	/// <summary>
	/// コマンドリスト
	/// </summary>
	/// <param name="barrierDesc"></param>
	/// <param name="pResource"></param>
	/// <param name="StateBefore"></param>
	/// <param name="StateAfter"></param>
	/// <param name="rtvHandle"></param>
	/// <param name="dsvHandle"></param>
	/// <param name="clearColor"></param>
	void CmdListDrawAble(D3D12_RESOURCE_BARRIER& barrierDesc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter,
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, FLOAT* clearColor = nullptr);
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="win_"></param>
	void Initialize(Window* win_);

	/// <summary>
	/// スクリーン描画前準備
	/// </summary>
	void PrevDrawScreen();

	/// <summary>
	/// スクリーン描画後始末
	/// </summary>
	void PostDrawScreen();

	/// <summary>
	/// 描画前準備
	/// </summary>
	void PrevDraw(FLOAT* clearColor = nullptr);

	/// <summary>
	/// 描画後始末
	/// </summary>
	void PostDraw();

	/// <summary>
	/// 深度クリア
	/// </summary>
	void ClearDepthBuff();

	//画像読み込み
	size_t LoadTextureGraph(const wchar_t* textureName);

	/// <summary>
	/// テクスチャハンドルを返す
	/// </summary>
	/// <param name="handle"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureHandle(size_t handle);

	/// <summary>
	/// デバイスを返す
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDev() { return device.Get(); }

	/// <summary>
	/// コマンドリスト
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

	/// <summary>
	/// viewPortを返す
	/// </summary>
	/// <returns></returns>
	Matrix GetViewportMat() { return viewPort.Mat(); }

	/// <summary>
	/// インクリメントサイズを返す
	/// </summary>
	/// <returns></returns>
	UINT GetIncrementSize() { return incrementSize; }

	/// <summary>
	/// windowを返す
	/// </summary>
	/// <returns></returns>
	Window GetWindow() { return *win; };

	/// <summary>
	/// バックバッファの数を取得
	/// </summary>
	/// <returns></returns>
	size_t GetBackByfferCount() const { return backBuffers.size(); };

	//シングルトン
	static MyDirectX* GetInstance();
};

