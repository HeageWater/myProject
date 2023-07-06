#include "PEffect.h"
#include <d3dx12.h>

using namespace DirectX;

PEffect::PEffect(SpriteCommon* spriteCommon,DirectXCommon* dxCommon):Sprite()
{
	//spriteCommon_ = new SpriteCommon;
	spriteCommon_ = spriteCommon;

	Sprite::Inilialize(spriteCommon);
}

void PEffect::Draw()
{
	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCommandList()->SetPipelineState(spriteCommon_->pipelineState);
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->rootSignature);

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	//�O�p�`���X�g
	//spriteCommon_->dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteCommon_->dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//�f�X�N���v�^
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1,);

	//�摜��1�ɓ��ꂽ���̂�
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, handle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0,constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void PEffect::Initialize()
{
	//HRESULT result;

	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	//	WindowApi::window_width,
	//	(UINT)WindowApi::window_height,
	//	1, 0, 1, 0,
	//	D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//CD3DX12_HEAP_PROPERTIES  properities = {};
	//properities.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//properities.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//properities.Type = D3D12_HEAP_TYPE_CUSTOM;

	//result = device->CreateCommittedResource(
	//	&properities,
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
	//	&value,
	//	IID_PPV_ARGS(&texBuff)
	//);
	//assert(SUCCEEDED(result));
}

void PEffect::Update(XMMATRIX& matView, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���Ȃǂ̌v�Z
	matScale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(this->rotation.z);
	matRot *= XMMatrixRotationX(this->rotation.x);
	matRot *= XMMatrixRotationY(this->rotation.y);

	matTrans = XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	this->matWorld = XMMatrixIdentity();
	this->matWorld *= matScale;
	this->matWorld *= matRot;
	this->matWorld *= matTrans;

	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / WindowApi::window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / WindowApi::window_height;

	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

	XMMATRIX matProjection1 = XMMatrixIdentity();

	////�ˉe�ϊ��s��
	//matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),
	//	(float)spriteCommon_->dxCommon_->GetWindow()->window_width / spriteCommon_->dxCommon_->GetWindow()->window_height,
	//	0.1f, 1000.0f);

	//���̂܂Ƃ�
	matProjection1 = XMMatrixOrthographicOffCenterLH
	(0,
		(float)WindowApi::Get()->window_width,
		(float)WindowApi::Get()->window_height,
		0,
		0.0f,
		1.0f);

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	constMapTransform->mat = this->matWorld * matProjection1;

	constBuffTransform->Unmap(0, nullptr);
}
