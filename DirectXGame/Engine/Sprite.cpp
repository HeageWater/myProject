#include "Sprite.h"
#include <DirectXTex.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	//delete spriteCommon_;
}

void Sprite::Inilialize(SpriteCommon* spriteCommon, Matrix* matProjection_)
{
	//spriteCommon_ = new SpriteCommon;
	this->spriteCommon_ = spriteCommon;
	this->matProjection_ = *matProjection_;

	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	//UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//UINT sizeVB = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//VBInitialize(dx->GetDev(), sizePV, vertexSize);
	//���_�o�b�t�@�̐ݒ�
	//�q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPU�ւ̓]���p
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	//resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = spriteCommon->dxCommon_->GetDev()->CreateCommittedResource(
		//�q�[�v�ݒ�
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//gpu��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	//ScreenVertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//�S���_�ɑ΂���
	//for (size_t i = 0; i < vertexSize; i++)
	for (size_t i = 0; i < _countof(vertices); i++)
	{
		//���W�R�s�[
		//vertMap[i] = pv[i];
		vertMap[i] = vertices[i];
	}

	//�Ȃ�����폜
	vertBuff->Unmap(0, nullptr);

	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;

	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResouceDesc{};
	cbResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResouceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResouceDesc.Height = 1;
	cbResouceDesc.DepthOrArraySize = 1;
	cbResouceDesc.MipLevels = 1;
	cbResouceDesc.SampleDesc.Count = 1;
	cbResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = spriteCommon->dxCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	//ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	//�l���������ނƎ����I�ɓ]������� 
	constMapTransform->color = MyMath::float4(1, 1, 1, 0.5f);	//���ʂ̐F

	constMapTransform->mat = Matrix();

	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	UINT sizeIB = static_cast<UINT>(sizeof(Vertex) * _countof(indices));

	//�C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	result = spriteCommon_->dxCommon_->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	//ScreenVertex* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (size_t i = 0; i < _countof(indices); i++)
	{
		//�C���f�b�N�X���R�s�[
		indexMap[i] = indices[i];
	}
	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

void Sprite::Update()
{
	constMapTransform->mat.Identity();

	//Initialize
	Matrix matScale, matRot, matTrans;
	this->matWorld.Identity();

	//scale
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;

	//rot
	matTrans.Identity();
	Matrix matRotX;
	matRotX.Identity();
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//trans
	matTrans.Identity();
	matTrans.m[3][0] = position.x;
	matTrans.m[3][1] = position.y;
	matTrans.m[3][2] = position.z;

	//����
	this->matWorld = matScale;
	this->matWorld *= matRot;
	this->matWorld *= matTrans;

	//////�ˉe�ϊ��s��
	matProjection_ = MyMath::OrthoLH
	(Window::window_width,
		Window::window_height,
		0.0f,
		1.0f);

	//constMapTransform->color = color;

	constMapTransform->mat.m[0][0] = 2.0f / Window::window_width;
	constMapTransform->mat.m[1][1] = -2.0f / Window::window_height;

	constMapTransform->mat *= matWorld;
	constMapTransform->mat *= matProjection_;

	constMapTransform->mat.m[1][1] = -constMapTransform->mat.m[1][1];

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	constBuffTransform->Unmap(0, nullptr);
}

void Sprite::PreDraw()
{
	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCmdList()->SetPipelineState(spriteCommon_->pipelineState);
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootSignature(spriteCommon_->rootSignature);

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	//�O�p�`���X�g
	//spriteCommon_->dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteCommon_->dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::Draw(size_t handle)
{
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	//�f�X�N���v�^
	//spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1,);

	//�摜�������ɓ��ꂽ���̂�
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1, spriteCommon_->dxCommon_->GetTextureHandle(handle));

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCmdList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::Draw()
{
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	//�摜�������ɓ��ꂽ���̂�
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1, spriteCommon_->dxCommon_->GetTextureHandle(tex));

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCmdList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::LoadFile(const wchar_t* textureName)
{
	tex = spriteCommon_->dxCommon_->LoadTextureGraph(textureName);
}

void Sprite::TransferSpriteVertex(Vector2D size_)
{
	//size = size_;

	//// �����A����A�E���A�E��
	//enum { LB, LT, RB, RT };

	//float left = 0.0f * size.x;
	//float right = 1.0f * size.x;
	//float top = 0.0f * size.y;
	//float bottom = 1.0f * size.y;

	//pv[LB].pos = { left,	bottom,	0.0f }; // ����
	//pv[LT].pos = { left,	top,	0.0f }; // ����
	//pv[RB].pos = { right,	bottom,	0.0f }; // �E��
	//pv[RT].pos = { right,	top,	0.0f }; // �E��

	//float tex_left = 0.0f;
	//float tex_right = 1.0f;
	//float tex_top = 0.0f;
	//float tex_bottom = 1.0f;

	//pv[LB].uv = { tex_left,	tex_bottom }; // ����
	//pv[LT].uv = { tex_left,	tex_top }; // ����
	//pv[RB].uv = { tex_right,	tex_bottom }; // �E��
	//pv[RT].uv = { tex_right,	tex_top }; // �E��
}