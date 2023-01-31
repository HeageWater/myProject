#include "VertBuff.h"
#include <wrl.h>
#include <vector>

VertBuff::VertBuff()
{
}

void VertBuff::SetLighting()
{
	//for (int i = 0; i < indicesSize / 3; i++)
	//{
	//	unsigned short index0 = indices[i * 3];
	//	unsigned short index1 = indices[i * 3 + 1];
	//	unsigned short index2 = indices[i * 3 + 2];

	//	Vector4D p0(vertices[index0].pos, 0.0f);
	//	Vector4D p1(vertices[index1].pos, 0.0f);
	//	Vector4D p2(vertices[index2].pos, 0.0f);

	//	Vector4D v1 = p1 - p0;
	//	Vector4D v2 = p2 - p0;
	//	Vector4D normal = v1 - v2;

	//	normal.normalize();
	//	Vector3D normal3D(normal.x, normal.y, normal.z);
	//	vertices[index0].normal = normal3D;
	//	vertices[index1].normal = normal3D;
	//	vertices[index2].normal = normal3D;
	//}
}

void VertBuff::VertBuffUpdate(ID3D12GraphicsCommandList* cmdList)
{
	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//	�C���f�b�N�X�o�b�t�@�r���[�ݒ�R�}���h
	if (ibExist) {
		cmdList->IASetIndexBuffer(&ibView);
	}
}

void VertBuff::SetVertices()
{
}

void VertBuff::VBInitialize(ID3D12Device* dev, UINT sizeVB, UINT vertSize, UINT sizeIB, uint16_t* indices, UINT indicesSize)
{
	SetLighting();

	//	�q�[�v�̐ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p(CPU����A�N�Z�X�ł���)

#pragma region VB
	// ���\�[�X�ݒ�
	SetResDesc(sizeVB);

	//	GPU���Ƀ������m��
	result = dev->CreateCommittedResource(
		&heapProp,							// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,							// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�r���[�̍쐬(GPU�ŗ��p���邽��)
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;

	SetVertices();
#pragma endregion

#pragma region IB
	if (indices != nullptr) {
		ibExist = true;

		SetResDesc(sizeIB);
		result = dev->CreateCommittedResource(
			&heapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));
		assert(SUCCEEDED(result));
		//	�C���f�b�N�X�o�b�t�@�}�b�s���O
		uint16_t* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));
		// �S���_�ɑ΂���
		for (int i = 0; i < indicesSize; i++) {
			indexMap[i] = indices[i]; // ���W���R�s�[
		}
		// �q���������
		indexBuff->Unmap(0, nullptr);
		//	�C���f�b�N�X�o�b�t�@�r���[�쐬
		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;
	}
#pragma endregion
}

void VertBuff::SetResDesc(UINT size)
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = size; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;		//	�A���`�G�C���A�V���O�p�̃p�����[�^
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void VertBuff::BuffTransferGPU(ID3D12Resource* buff, ID3D12Device* dev)
{
#pragma region �o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));
#pragma endregion
}
