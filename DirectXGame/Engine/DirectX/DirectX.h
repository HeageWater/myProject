#pragma once
#include "Window.h"
#include "ViewPort.h"
#include "ScissorRect.h"
#include "DescriptorHeap.h"
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

	/// <summary>
	/// デバッグレイヤー
	/// </summary>
	void DebugLayer();

	/// <summary>
	/// FPS初期化
	/// </summary>
	void InitializeFPS();

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
	/// スクリーン描画前準備(clearColorSet)
	/// </summary>
	void PrevDrawScreen(FLOAT* clearColor_);

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

	/// <summary>
	/// FPS更新
	/// </summary>
	void UpdateFPS();

	/// <summary>
	/// 画像読み込み
	/// </summary>
	/// <param name="textureName"></param>
	/// <returns></returns>
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
	ID3D12Device* GetDev() { return device_.Get(); }

	/// <summary>
	/// コマンドリスト
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList_.Get(); }

	/// <summary>
	/// viewPortを返す
	/// </summary>
	/// <returns></returns>
	Matrix GetViewportMat() { return viewPort_.Mat(); }

	/// <summary>
	/// インクリメントサイズを返す
	/// </summary>
	/// <returns></returns>
	UINT GetIncrementSize() { return incrementSize_; }

	/// <summary>
	/// windowを返す
	/// </summary>
	/// <returns></returns>
	Window GetWindow() { return *win_; };

	/// <summary>
	/// バックバッファの数を取得
	/// </summary>
	/// <returns></returns>
	size_t GetBackByfferCount() const { return backBuffers_.size(); };

	/// <summary>
	///シングルトン
	/// </summary>
	/// <returns></returns>
	static MyDirectX* GetInstance();

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
	/// デスクリプタヒープを返す
	/// </summary>
	DescriptorHeap* GetDescriptorHeap();

private:
	Window* win_ = nullptr;

	HRESULT result_;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Device> device_;

	ComPtr<ID3D12CommandAllocator> cmdAllocator_;
	ComPtr<ID3D12GraphicsCommandList> cmdList_;

	ComPtr<ID3D12CommandQueue> cmdQueue_;

	ComPtr<IDXGISwapChain4> swapChain_;

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	// バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle_;

	ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	D3D12_RESOURCE_BARRIER barrierDesc_{};

	FLOAT clearColor_[4] = {};

	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;

	ComPtr<ID3D12Resource> depthBuff_;

	// screenTexture
	ComPtr<ID3D12Resource> screenResource_;
	D3D12_RESOURCE_BARRIER screenBarrierDesc_;
	ComPtr<ID3D12DescriptorHeap> screenRTVHeap_;
	std::vector<ComPtr<ID3D12DescriptorHeap>> screenSRVHeap_;

	//	ビューポート
	ViewPort viewPort_;
	// シザー矩形
	ScissorRect scissorRect_;

	int textureNum_;
	std::vector<ComPtr<ID3D12Resource>> texBuff_;
	UINT incrementSize_;

	std::chrono::steady_clock::time_point reference_;

	//デスクプリタヒープ
	std::unique_ptr<DescriptorHeap> descriptorHeap_;
};

