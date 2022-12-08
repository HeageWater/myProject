#include "DirectX.h"
#include "Window.h"
#pragma comment (lib, "winmm.lib")
#include <DirectXTex.h>
using namespace DirectX;

// �Ή����x���̔z��
D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};

void MyDirectX::DebugLayer()
{
	//	�I��
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

MyDirectX::MyDirectX(Window* win_)
{
	Initialize(win_);
}

void MyDirectX::Initialize(Window* win_)
{
	win = win_;

#ifdef _DEBUG
	DebugLayer();
#endif

#pragma region GPU��
#pragma region Adapter
	ComPtr<IDXGIFactory7> dxgiFactory;
	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}

	//	�O���{�A�_�v�^�I��
	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);
		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i].Get();
			break;
		}
	}
#pragma endregion Adapter

#pragma region Device
	//	�f�o�C�X����
	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion Device
#pragma endregion

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	device.As(&infoQueue);

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

	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif

#pragma region CmdList
	// �R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�𐶐�
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));
	assert(SUCCEEDED(result));
#pragma endregion CmdList

#pragma region CmdQueue
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&cmdQueue));
	assert(SUCCEEDED(result));
#pragma endregion CmdQueue

#pragma region DoubleBuffering
#pragma region swapChain
	ComPtr<IDXGISwapChain1> swapChain1;
	// �ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;										//	��ʕ��𑜓x
	swapChainDesc.Height = 720;										//	��ʍ����𑜓x
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// �F���̏���
	swapChainDesc.SampleDesc.Count = 1;								// �}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;				// �o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;									// �o�b�t�@����2�ɐݒ�(�\�Ɨ�)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// �t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	// �X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(), win->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapChain1);

	swapChain1.As(&swapChain);		//	1��4�ɕϊ�
	assert(SUCCEEDED(result));
#pragma endregion swapChain

#pragma region DesHeap
	// �f�X�N���v�^�q�[�v�̐ݒ�(GPU�Ƀ������̈�m�ۂ����ꂩ��g��)
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��
	// �f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
#pragma endregion DesHeap

#pragma region BackBuff
	backBuffers.resize(swapChainDesc.BufferCount);
#pragma endregion BackBuff

#pragma region RTV
	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++) {
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾(�擪�A�h���X)
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
#pragma endregion RTV
#pragma endregion

#pragma region �}���`�p�X�����_�����O
	auto& bbuff = backBuffers[0];
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

	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(screenResource.ReleaseAndGetAddressOf()));

#pragma region RTV
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = rtvHeap->GetDesc();
	//	heap
	heapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(screenRTVHeap.ReleaseAndGetAddressOf()));

	D3D12_RENDER_TARGET_VIEW_DESC _rtvDesc = {};
	_rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	_rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//	RTV
	device->CreateRenderTargetView(
		screenResource.Get(),
		&_rtvDesc,
		screenRTVHeap.Get()->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
#pragma region SRV
	textureNum = 0;
	const size_t kMaxSRVCount = 2056;

	//	heapSETTING
	heapDesc.NumDescriptors = kMaxSRVCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//	vector::resize
	screenSRVHeap.resize(heapDesc.NumDescriptors);
	texBuff.resize(heapDesc.NumDescriptors);

	result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(screenSRVHeap[0].ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc = {};
	_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	_srvDesc.Format = _rtvDesc.Format;
	_srvDesc.Texture2D.MipLevels = 1;
	_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//	SRV
	device->CreateShaderResourceView(
		screenResource.Get(),
		&_srvDesc,
		screenSRVHeap[0]->GetCPUDescriptorHandleForHeapStart());
#pragma endregion
#pragma endregion

#pragma region �[�x�o�b�t�@
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Window::window_width;
	depthResourceDesc.Height = Window::window_height;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//	�[�x�n�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//	�[�x�n�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	//	Resource����
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
	//	�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//	view
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
#pragma endregion

#pragma region fence
	// �t�F���X�̐���
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion fence

	//	�r���[�|�[�g
	viewPort.Init(Window::window_width, Window::window_height, 0, 0);
	// �V�U�[��`
	scissorRect.Init(0, Window::window_width, 0, Window::window_height);
}

void MyDirectX::SetResourceBarrier(D3D12_RESOURCE_BARRIER& desc, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, ID3D12Resource* pResource)
{
	if (pResource != nullptr) {
		desc.Transition.pResource = pResource;
	}
	desc.Transition.StateBefore = StateBefore;
	desc.Transition.StateAfter = StateAfter;
	cmdList->ResourceBarrier(1, &desc);
}

void MyDirectX::PrevDraw(FLOAT* clearColor)
{
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
#pragma region ReleaseBarrier
	// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
	UINT64 bbIndex = swapChain->GetCurrentBackBufferIndex();		//	���݂̃o�b�N�o�b�t�@�ݒ�
#pragma endregion ReleaseBarrier
	
	// 2.�`���̕ύX
#pragma region Change
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
#pragma endregion Change

	CmdListDrawAble(barrierDesc, backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET, rtvHandle, dsvHandle, clearColor);
}

void MyDirectX::CmdListDrawAble(D3D12_RESOURCE_BARRIER& desc, ID3D12Resource* pResource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, FLOAT* clearColor)
{
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
#pragma region ReleaseBarrier
	SetResourceBarrier(desc, StateBefore, StateAfter, pResource);
#pragma endregion ReleaseBarrier
	// 2.�`���̕ύX
#pragma region Change
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion Change
	// 3.��ʃN���A
#pragma region ScreenClear
	if (clearColor == nullptr) {
		ScreenClear(rtvHandle);
	}
	else {
		ScreenClear(clearColor, rtvHandle);
	}
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion
}

void MyDirectX::PostDraw()
{
	// 5.���\�[�X�o���A��߂�
#pragma region ReleaseBarrier
	SetResourceBarrier(barrierDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
#pragma endregion ReleaseBarrier

	// ���߂̃N���[�Y
#pragma region CmdClose
	result = cmdList->Close();
	assert(SUCCEEDED(result));
	// ���߂Ă����R�}���h���X�g�̎��s(close�K�{)
	ID3D12CommandList* commandLists[] = { cmdList.Get()};
	cmdQueue->ExecuteCommandLists(1, commandLists);
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
#pragma endregion CmdClose

#pragma region ChangeScreen
	// �R�}���h�̎��s������҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)	//	GPU�̏�������������������
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0) {
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// �L���[���N���A
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h���X�g�𒙂߂鏀��
	result = cmdList->Reset(cmdAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
#pragma endregion ChangeScreen
}

void MyDirectX::PrevDrawScreen(FLOAT* clearColor)
{
	rtvHandle = screenRTVHeap->GetCPUDescriptorHandleForHeapStart();
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	CmdListDrawAble(screenBarrierDesc, screenResource.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET,
		rtvHandle, dsvHandle);

	viewPort.Update(cmdList.Get());

	scissorRect.Update(cmdList.Get());

	cmdList->SetDescriptorHeaps(1, screenSRVHeap[0].GetAddressOf());
}

void MyDirectX::PostDrawScreen()
{
	SetResourceBarrier(screenBarrierDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

int MyDirectX::LoadTextureGraph(const wchar_t* textureName)
{
	textureNum++;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	HRESULT result = LoadFromWICFile(
		textureName,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	//	�~�j�}�b�v����
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
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

	//	�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	int buffIndex = textureNum - 1;
	//	�e�N�X�`���o�b�t�@����
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&tectureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff[buffIndex]));
	//	�e�N�X�`���o�b�t�@�]��
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);

		result = texBuff[buffIndex]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

#pragma region SetSRV
	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = screenSRVHeap[0]->GetCPUDescriptorHandleForHeapStart();
	srvHandle.ptr += incrementSize * textureNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texBuff[buffIndex].Get(), &srvDesc, srvHandle);
#pragma endregion

	return textureNum;
}

D3D12_GPU_DESCRIPTOR_HANDLE MyDirectX::GetTextureHandle(int handle)
{
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = screenSRVHeap[0]->GetGPUDescriptorHandleForHeapStart();
	srvGpuHandle.ptr += incrementSize * handle;
	return srvGpuHandle;
}

void MyDirectX::ScreenClear(FLOAT* clearColor, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle)
{
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}
void MyDirectX::ScreenClear(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle)
{
	FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f };
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

