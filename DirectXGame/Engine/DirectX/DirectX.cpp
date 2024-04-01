#include "DirectX.h"
#include "Window.h"
#pragma comment (lib, "winmm.lib")
#include <DirectXTex.h>
using namespace DirectX;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// 対応レベルの配列
D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};

void MyDirectX::DebugLayer()
{
	//	オン
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

void MyDirectX::InitializeFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void MyDirectX::UpdateFPS()
{
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	if (elapsed < kMinCheckTime) {
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	reference_ = std::chrono::steady_clock::now();
}

void MyDirectX::Initialize(Window* win)
{
	win_ = win;

#ifdef _DEBUG
	DebugLayer();
#endif

#pragma region GPU列挙
#pragma region Adapter
	ComPtr<IDXGIFactory7> dxgiFactory;
	// DXGIファクトリーの生成
	result_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result_));
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	//	グラボアダプタ選別
	// 妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i].Get();
			break;
		}
	}
#pragma endregion Adapter

#pragma region Device
	//	デバイス生成
	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		// 採用したアダプターでデバイスを生成
		result_ = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result_ == S_OK) {
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion Device
#pragma endregion

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	device_.As(&infoQueue);

	D3D12_MESSAGE_ID denyIds[] = {
	  D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
	};
	D3D12_MESSAGE_SEVERITY severities[] = {
	  D3D12_MESSAGE_SEVERITY_INFO
	};
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;

	infoQueue->PushStorageFilter(&filter);

	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif

#pragma region CmdList
	// コマンドアロケータを生成
	result_ = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator_));
	assert(SUCCEEDED(result_));
	// コマンドリストを生成
	result_ = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&cmdList_));
	assert(SUCCEEDED(result_));
#pragma endregion CmdList

#pragma region CmdQueue
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result_ = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&cmdQueue_));
	assert(SUCCEEDED(result_));
#pragma endregion CmdQueue

#pragma region DoubleBuffering
#pragma region swapChain
	ComPtr<IDXGISwapChain1> swapChain1;
	// 設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;										//	画面幅解像度
	swapChainDesc.Height = 720;										//	画面高さ解像度
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// 色情報の書式
	swapChainDesc.SampleDesc.Count = 1;								// マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				// バックバッファ用
	swapChainDesc.BufferCount = 2;									// バッファ数を2つに設定(表と裏)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// スワップチェーンの生成
	result_ = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue_.Get(), win_->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapChain1);

	swapChain1.As(&swapChain_);		//	1→4に変換
	assert(SUCCEEDED(result_));
#pragma endregion swapChain

#pragma region DesHeap
	// デスクリプタヒープの設定(GPUにメモリ領域確保しそれから使う)
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc_.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ
	// デスクリプタヒープの生成
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));
#pragma endregion DesHeap

#pragma region BackBuff
	backBuffers_.resize(swapChainDesc.BufferCount);
#pragma endregion BackBuff

#pragma region RTV
	// スワップチェーンの全てのバッファについて処理する
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		// スワップチェーンからバッファを取得
		swapChain_->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers_[i]));
		// デスクリプタヒープのハンドルを取得(先頭アドレス)
		rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle_.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle_);
	}
#pragma endregion RTV
#pragma endregion

#pragma region マルチパスレンダリング
	auto& bbuff = backBuffers_[0];
	auto resDesc = bbuff->GetDesc();

	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	float clsClr[4] = { 0.5f,0.5f,0.5f,1.0f };
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	clearValue.DepthStencil.Depth = 0.5f;
	for (size_t i = 0; i < 4; i++)
	{
		clearValue.Color[i] = clsClr[i];
	}

	result_ = device_->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(screenResource_.ReleaseAndGetAddressOf()));

#pragma region RTV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = rtvHeap_->GetDesc();
	//	heap
	heapDesc.NumDescriptors = 1;
	result_ = device_->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(screenRTVHeap_.ReleaseAndGetAddressOf()));

	D3D12_RENDER_TARGET_VIEW_DESC _rtvDesc = {};
	_rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	_rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//	RTV
	device_->CreateRenderTargetView(
		screenResource_.Get(),
		&_rtvDesc,
		screenRTVHeap_.Get()->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
#pragma region SRV
	textureNum_ = 0;
	const size_t kMaxSRVCount = 2056;

	//	heapSETTING
	heapDesc.NumDescriptors = kMaxSRVCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//	vector::resize
	screenSRVHeap_.resize(heapDesc.NumDescriptors);
	texBuff_.resize(heapDesc.NumDescriptors);

	result_ = device_->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(screenSRVHeap_[0].ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result_));

	D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
	_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	_srvDesc.Format = _rtvDesc.Format;
	_srvDesc.Texture2D.MipLevels = 1;
	_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//	SRV
	device_->CreateShaderResourceView(
		screenResource_.Get(),
		&_srvDesc,
		screenSRVHeap_[0]->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
#pragma endregion

#pragma region 深度バッファ
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Window::window_width_;
	depthResourceDesc.Height = Window::window_height_;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//	深度地用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//	深度地のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	//	Resource生成
	result_ = device_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));
	//	デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result_ = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));
	//	view
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart());
#pragma endregion

#pragma region fence
	// フェンスの生成
	result_ = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
#pragma endregion fence

	//	ビューポート
	viewPort_.Init(Window::window_width_, Window::window_height_, 0, 0);
	// シザー矩形
	scissorRect_.Init(0, Window::window_width_, 0, Window::window_height_);

	//FPS初期設定
	InitializeFPS();
}

void MyDirectX::SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource)
{
	if (pResource != nullptr) {
		desc.Transition.pResource = pResource;
	}
	desc.Transition.StateBefore = StateBefore;
	desc.Transition.StateAfter = StateAfter;
	cmdList_->ResourceBarrier(1, &desc);
}

void MyDirectX::PrevDraw(FLOAT* clearColor)
{
	// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
	// バックバッファの番号を取得(2つなので0番か1番)
	UINT64 bbIndex = swapChain_->GetCurrentBackBufferIndex();		//	現在のバックバッファ設定
#pragma endregion ReleaseBarrier

	// 2.描画先の変更
#pragma region Change
	// レンダーターゲットビューのハンドルを取得
	rtvHandle_ = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle_.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
	dsvHandle_ = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
#pragma endregion Change

	CmdListDrawAble(barrierDesc_, backBuffers_[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET, rtvHandle_, dsvHandle_, clearColor);
}

void MyDirectX::CmdListDrawAble(D3D12_RESOURCE_BARRIER& desc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, FLOAT* clearColor)
{
	// 1.リソースバリアで書き込み可能に変更
#pragma region ReleaseBarrier
	SetResourceBarrier(desc, StateBefore, StateAfter, pResource);
#pragma endregion ReleaseBarrier
	// 2.描画先の変更
#pragma region Change
	cmdList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion Change
	// 3.画面クリア
#pragma region ScreenClear
	if (clearColor == nullptr) {
		ScreenClear(rtvHandle);
	}
	else {
		ScreenClear(clearColor, rtvHandle);
	}
	cmdList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion
}

void MyDirectX::PostDraw()
{
	// 5.リソースバリアを戻す
#pragma region ReleaseBarrier
	SetResourceBarrier(barrierDesc_, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
#pragma endregion ReleaseBarrier

	// 命令のクローズ
#pragma region CmdClose
	result_ = cmdList_->Close();
	assert(SUCCEEDED(result_));
	// 溜めていたコマンドリストの実行(close必須)
	ID3D12CommandList* commandLists[] = { cmdList_.Get() };
	cmdQueue_->ExecuteCommandLists(1, commandLists);
	// 画面に表示するバッファをフリップ(裏表の入替え)
	result_ = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result_));
#pragma endregion CmdClose

#pragma region ChangeScreen
	// コマンドの実行完了を待つ
	cmdQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_)	//	GPUの処理が完了したか判定
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		if (event != 0) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// キューをクリア
	result_ = cmdAllocator_->Reset();
	assert(SUCCEEDED(result_));
	// 再びコマンドリストを貯める準備
	result_ = cmdList_->Reset(cmdAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result_));
#pragma endregion ChangeScreen
}

void MyDirectX::PrevDrawScreen()
{
	rtvHandle_ = screenRTVHeap_->GetCPUDescriptorHandleForHeapStart();
	dsvHandle_ = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	CmdListDrawAble(screenBarrierDesc_, screenResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET,
		rtvHandle_, dsvHandle_);

	viewPort_.Update(cmdList_.Get());

	scissorRect_.Update(cmdList_.Get());

	cmdList_->SetDescriptorHeaps(1, screenSRVHeap_[0].GetAddressOf());
}

void MyDirectX::PrevDrawScreen(FLOAT* clearColor)
{
	rtvHandle_ = screenRTVHeap_->GetCPUDescriptorHandleForHeapStart();
	dsvHandle_ = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	CmdListDrawAble(screenBarrierDesc_, screenResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET,
		rtvHandle_, dsvHandle_, clearColor);

	viewPort_.Update(cmdList_.Get());

	scissorRect_.Update(cmdList_.Get());

	cmdList_->SetDescriptorHeaps(1, screenSRVHeap_[0].GetAddressOf());
}

void MyDirectX::PostDrawScreen()
{
	SetResourceBarrier(screenBarrierDesc_, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

size_t MyDirectX::LoadTextureGraph(const wchar_t* textureName)
{
	textureNum_++;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result_ = LoadFromWICFile(
		textureName,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	//	ミニマップ生成
	ScratchImage mipChain{};
	result_ = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result_)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);

	D3D12_RESOURCE_DESC tectureResourceDesc{};
	tectureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	tectureResourceDesc.Format = metadata.format;
	tectureResourceDesc.Width = metadata.width;
	tectureResourceDesc.Height = (UINT)metadata.height;
	tectureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	tectureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	tectureResourceDesc.SampleDesc.Count = 1;

	//	ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	int buffIndex = textureNum_ - 1;
	//	テクスチャバッファ生成
	result_ = device_->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&tectureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff_[buffIndex]));
	//	テクスチャバッファ転送
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		result_ = texBuff_[buffIndex]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result_));
	}

#pragma region SetSRV
	incrementSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = screenSRVHeap_[0]->GetCPUDescriptorHandleForHeapStart();
	srvHandle.ptr += incrementSize_ * textureNum_;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device_->CreateShaderResourceView(texBuff_[buffIndex].Get(), &srvDesc, srvHandle);
#pragma endregion

	return textureNum_;
}

D3D12_GPU_DESCRIPTOR_HANDLE MyDirectX::GetTextureHandle(size_t handle)
{
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = screenSRVHeap_[0]->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += incrementSize_ * handle;
	return srvGpuHandle;
}

MyDirectX* MyDirectX::GetInstance()
{
	static MyDirectX dirextX;
	return &dirextX;
}

void MyDirectX::ScreenClear(FLOAT* clearColor, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle)
{
	cmdList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void MyDirectX::ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle)
{
	FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f };
	cmdList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

DescriptorHeap* MyDirectX::GetDescriptorHeap()
{
	return descriptorHeap_.get();
}

void MyDirectX::ClearDepthBuff()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	// 深度バッファのクリア
	cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}