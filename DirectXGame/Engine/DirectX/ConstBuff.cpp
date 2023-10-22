#include "ConstBuff.h"
#include "Vector3D.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")

ConstBuff::ConstBuff(ID3D12Device* dev)
{
	//	ヒープ設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	resourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc_.Height = 1;
	resourceDesc_.DepthOrArraySize = 1;
	resourceDesc_.MipLevels = 1;
	resourceDesc_.SampleDesc.Count = 1;
	resourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc_.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	result_ = dev->CreateCommittedResource(
		&heapProp_,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc_,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_));
	assert(SUCCEEDED(result_));

	//	定数バッファのマッピング
	result_ = material_->Map(0, nullptr, (void**)&mapMaterial_);	//	マッピング
	assert(SUCCEEDED(result_));


	//	GPUのメモリにデータ転送
	ChangeColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));

	resourceDesc_.Width = (sizeof(ConstBufferTimeMaterial) + 0xFF) & ~0xFF;
	//	生成
	result_ = dev->CreateCommittedResource(
		&heapProp_,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc_,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&timeMaterial_));
	assert(SUCCEEDED(result_));

	//	定数バッファのマッピング
	result_ = timeMaterial_->Map(0, nullptr, (void**)&timeMapMaterial_);	//	マッピング
	assert(SUCCEEDED(result_));


	//	GPUのメモリにデータ転送
	timeMapMaterial_->time_ = 0;
}

ConstBuff::~ConstBuff()
{
	material_->Unmap(0, nullptr);
	timeMaterial_->Unmap(0, nullptr);
}

void ConstBuff::Initialize(ID3D12Device* dev)
{//	ヒープ設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	resourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc_.Height = 1;
	resourceDesc_.DepthOrArraySize = 1;
	resourceDesc_.MipLevels = 1;
	resourceDesc_.SampleDesc.Count = 1;
	resourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc_.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	result_ = dev->CreateCommittedResource(
		&heapProp_,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc_,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_));
	assert(SUCCEEDED(result_));

	//	定数バッファのマッピング
	result_ = material_->Map(0, nullptr, (void**)&mapMaterial_);	//	マッピング
	assert(SUCCEEDED(result_));


	//	GPUのメモリにデータ転送
	ChangeColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));

	resourceDesc_.Width = (sizeof(ConstBufferTimeMaterial) + 0xFF) & ~0xFF;
	//	生成
	result_ = dev->CreateCommittedResource(
		&heapProp_,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc_,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&timeMaterial_));
	assert(SUCCEEDED(result_));

	//	定数バッファのマッピング
	result_ = timeMaterial_->Map(0, nullptr, (void**)&timeMapMaterial_);	//	マッピング
	assert(SUCCEEDED(result_));


	//	GPUのメモリにデータ転送
	timeMapMaterial_->time_ = 0;
}

void ConstBuff::ChangeColor(const Vector4D& color)
{
	//	GPUのメモリにデータ転送
	mapMaterial_->color_ = color;
}

void ConstBuff::TimeUpdate()
{
	timeMapMaterial_->time_ += 0.1f;
}

void ConstBuff::SetBufferView(ID3D12GraphicsCommandList* cmdList)
{
	//	定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(1, timeMaterial_->GetGPUVirtualAddress());
}

void ConstBuff::Update(ID3D12GraphicsCommandList* cmdList)
{
	//	定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, material_->GetGPUVirtualAddress());
}
