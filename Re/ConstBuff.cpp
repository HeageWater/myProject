#include "ConstBuff.h"
#include "Vector3D.h"
#include <cassert>

ConstBuff::ConstBuff(ID3D12Device* dev, const size_t winwidth, const size_t winheight)
{
	//	�q�[�v�ݒ�
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	resourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc_.Height = 1;
	resourceDesc_.DepthOrArraySize = 1;
	resourceDesc_.MipLevels = 1;
	resourceDesc_.SampleDesc.Count = 1;
	resourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc_.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	����
	result_ = dev->CreateCommittedResource(
		&heapProp_,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc_,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_));
	assert(SUCCEEDED(result_));

	//	�萔�o�b�t�@�̃}�b�s���O
	result_ = material_->Map(0, nullptr, (void**)&mapMaterial_);	//	�}�b�s���O
	assert(SUCCEEDED(result_));


	//	GPU�̃������Ƀf�[�^�]��
	ChangeColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));

	resourceDesc_.Width = (sizeof(ConstBufferTimeMaterial) + 0xFF) & ~0xFF;
	//	����
	result_ = dev->CreateCommittedResource(
		&heapProp_,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc_,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&timeMaterial_));
	assert(SUCCEEDED(result_));

	//	�萔�o�b�t�@�̃}�b�s���O
	result_ = timeMaterial_->Map(0, nullptr, (void**)&timeMapMaterial_);	//	�}�b�s���O
	assert(SUCCEEDED(result_));


	//	GPU�̃������Ƀf�[�^�]��
	timeMapMaterial_->time_ = 0;
}

ConstBuff::~ConstBuff()
{
	material_->Unmap(0, nullptr);
	timeMaterial_->Unmap(0, nullptr);
}

void ConstBuff::ChangeColor(const Vector4D& color)
{
	//	GPU�̃������Ƀf�[�^�]��
	mapMaterial_->color_ = color;
}

void ConstBuff::TimeUpdate()
{
	timeMapMaterial_->time_ += 0.1f;
}

void ConstBuff::SetBufferView(ID3D12GraphicsCommandList* cmdList)
{
	//	�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(1, timeMaterial_->GetGPUVirtualAddress());
}

void ConstBuff::Update(ID3D12GraphicsCommandList* cmdList)
{
	//	�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, material_->GetGPUVirtualAddress());
}
