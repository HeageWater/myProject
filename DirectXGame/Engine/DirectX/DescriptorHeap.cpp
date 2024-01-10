#include "DescriptorHeap.h"
#include "DirectX.h"

void DescriptorHeap::Initialize()
{
	device_ = MyDirectX::GetInstance()->GetDev();

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = static_cast<uint32_t>(maxSRV_ + maxUAV_ + maxCBV_);

	// 設定を元にSRV用デスクリプタヒープを生成
	device_->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(descriptorHeap_.ReleaseAndGetAddressOf()));

	startCpuHandle_ = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	startGpuHandle_ = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();

	incrementSize_ = static_cast<uint32_t>(device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
}

UINT64 DescriptorHeap::CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (countSRV_ > maxSRV_)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle_.ptr + (static_cast<SIZE_T>(countSRV_) * incrementSize_);
	handle.gpuHandle.ptr = startGpuHandle_.ptr + (static_cast<SIZE_T>(countSRV_) * incrementSize_);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device_->CreateShaderResourceView(resource, &desc, handle.cpuHandle);
	countSRV_++;

	return handle.gpuHandle.ptr;
}

UINT64 DescriptorHeap::CreateUAV(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (countUAV_ > maxUAV_)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle_.ptr + (static_cast<SIZE_T>(maxSRV_ + countUAV_) * incrementSize_);
	handle.gpuHandle.ptr = startGpuHandle_.ptr + (static_cast<SIZE_T>(maxSRV_ + countUAV_) * incrementSize_);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device_->CreateUnorderedAccessView(resource, nullptr, &desc, handle.cpuHandle);

	countUAV_++;
	return handle.gpuHandle.ptr;
}

UINT64 DescriptorHeap::CreateCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc)
{
	if (countCBV_ > maxCBV_)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle_.ptr + (static_cast<SIZE_T>(maxSRV_ + maxUAV_ + countCBV_) * incrementSize_);
	handle.gpuHandle.ptr = startGpuHandle_.ptr + (static_cast<SIZE_T>(maxSRV_ + maxUAV_ + countCBV_) * incrementSize_);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device_->CreateConstantBufferView(&desc, handle.cpuHandle);

	countCBV_++;

	return handle.gpuHandle.ptr;
}

DescriptorHeap::DescriptorHeapViewHandle DescriptorHeap::AddSRV()
{
	if (countSRV_ > maxSRV_)
	{
		assert(0);
	}

	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle_.ptr + (static_cast<SIZE_T>(countSRV_) * incrementSize_);
	handle.gpuHandle.ptr = startGpuHandle_.ptr + (static_cast<SIZE_T>(countSRV_) * incrementSize_);

	countSRV_++;

	return handle;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeap::GetHeap()
{
	return descriptorHeap_;
}