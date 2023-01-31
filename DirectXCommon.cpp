#include "DirectXCommon.h"

#include <vector>
#include <cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;
//std::vector<Microsoft::WRL::ComPtr<ID3D12Resource*>> backBuffers;
std::vector<ID3D12Resource*> backBuffers;

void DirectXCommon::Initialize()
{
	assert(window);

	this->window = window;

	port->ChengePort(this->window->window_width, this->window->window_height);

	//�f�o�C�X�̏�����
	InitializeDevice();

	//�R�}���h�̏�����
	InitializeComand();

	//�X���b�v�`�F�[���̏�����
	InitializeSwapchain();

	//�����_�[�^�[�Q�b�g�r���[�̏�����
	InitializeREnderTargetView();

	//�[�x�o�b�t�@�̏�����
	InitializeDeathBuffer();

	//�t�F���X�̏�����
	InitializeFence();
}

void DirectXCommon::InitializeDevice()
{
	//ComPtr<IDXGIFactory7> dxgiFactory;

	////�f�o�b�O���C���[
	//ComPtr<ID3D12Debug1> debugController;
	//if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
	//	debugController->EnableDebugLayer();
	//	debugController->SetEnableGPUBasedValidation(TRUE);
	//}

	//DXGI�t�@�N�g���[�̍쐬
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//�A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter4*> adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter4* tmdAdapter = nullptr;

	//�p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmdAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		//���I�z��ɒǉ�����
		adapters.push_back(tmdAdapter);
	}

	//�Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		//�A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		//�\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//�f�o�C�X���̗p���ă��[�v�𔲂���
			tmdAdapter = adapters[i];
			break;
		}
	}

	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmdAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
}

void DirectXCommon::InitializeComand()
{
	//�R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	assert(SUCCEEDED(result));

	//�R�}���h���X�g�𐶐�
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeSwapchain()
{
	ComPtr<IDXGISwapChain1>swapChain1;

	//�X���b�v�`�F�[���̐ݒ�
	swapChainDesc.Width = window->window_width;
	swapChainDesc.Height = window->window_height;

	//�F���
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//�}���`�T���v�����Ȃ�
	swapChainDesc.SampleDesc.Count = 1;

	//�o�b�N�o�b�t�@�p
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;

	//�o�b�t�@����2�ɐݒ�
	swapChainDesc.BufferCount = 2;

	//�t���b�v��͔j��
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//�X���b�v�`�F�[���̐���	
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), window->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		swapChain1.GetAddressOf());
	swapChain1.As(&swapChain);
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeREnderTargetView()
{
	//�����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//���\��2��
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;

	//�f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	//�o�b�N�o�b�t�@
	//std::vector<ID3D12Resource*> backBuffers;

	backBuffers.resize(swapChainDesc.BufferCount);

	//�����_�[�^�[�Q�b�g�r���[
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));

		//�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

		//�����\�ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		//�V�F�[�_�[�̌v�Z����SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		//�����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}
}

void DirectXCommon::InitializeDeathBuffer()
{
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResouceDesc{};
	depthResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResouceDesc.Width = WindowApi::window_width;
	depthResouceDesc.Height = WindowApi::window_height;
	depthResouceDesc.DepthOrArraySize = 1;
	depthResouceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResouceDesc.SampleDesc.Count = 1;
	depthResouceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

	//���\�[�X�ݒ�
	ID3D12Resource* depthBuff = nullptr;
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResouceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff, &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::InitializeFence()
{
	//�t�F���X�̐���
	//ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void DirectXCommon::PreDraw()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾(0�Ԃ�1��)
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	//1.���\�[�X�o���A�ŏ������݉\�ɕύX����
	//D3D12_RESOURCE_BARRIER barrierDesc{};

	//�o�b�N�o�b�t�@���w��
	barrierDesc.Transition.pResource = backBuffers[bbIndex];

	//�\����Ԃ���
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;

	//�`���Ԃ�
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList->ResourceBarrier(1, &barrierDesc);

	//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//�w�i
	FLOAT clearColor[] = { 0.0f,0.1f,0.5f,1.0f };

	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	////�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	port->DrawViewPort(commandList.Get());
	////�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	port->DrawScissor(commandList.Get());
}

void DirectXCommon::PostDraw()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾(0�Ԃ�1��)
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	//�`���Ԃ���
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//�\����Ԃ�
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	//���߂̃N���[�Y
	result = commandList->Close();
	assert(SUCCEEDED(result));

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { commandList.Get()};
	commandQueue->ExecuteCommandLists(1, commandLists);

	//�R�}���h�̎��s������҂�
	commandQueue->Signal(fence, ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//�L���[���N���A
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));

	//�ĂуR�}���h���X�g�����߂鏀��
	result = commandList->Reset(cmdAllocator, nullptr);
	assert(SUCCEEDED(result));

	//��ʕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
}
