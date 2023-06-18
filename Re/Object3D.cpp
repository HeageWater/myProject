#include "Object3D.h"
#include <cmath>
#include "MyMath.h"
#include <cassert>

void Object3D::Initialize(Shader shader)
{
	HRESULT newresult;

	D3D12_HEAP_PROPERTIES newheapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};

	//	�q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	����
	newresult = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(newresult));

	//	�萔�o�b�t�@�̃}�b�s���O
	newresult = transform->Map(0, nullptr, (void**)&constMapTransform);	//	�}�b�s���O
	assert(SUCCEEDED(newresult));

	vertexSize = 24;
	vertices.resize(vertexSize);
	// ���_�f�[�^
	vertices = {
		//	�O
		{{ -1.0f,-1.0f,-1.0f },{},{0.0f, 1.0f}}, // ����
		{{ -1.0f, 1.0f,-1.0f },{},{0.0f, 0.0f}}, // ����
		{{  1.0f,-1.0f,-1.0f },{},{1.0f, 1.0f}}, // �E��
		{{  1.0f, 1.0f,-1.0f },{},{1.0f, 0.0f}}, // �E��
		//	��
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 1.0f}}, // ����
		{{ -1.0f, 1.0f, 1.0f },{}, {0.0f, 0.0f}}, // ����
		{{  1.0f,-1.0f, 1.0f },{}, {1.0f, 1.0f}}, // �E��
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // �E��
		//	��
		{{ -1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // �E��
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // ����
		{{ -1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // ����
		{{ -1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // �E��
		//	�E
		{{  1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // ����
		{{  1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // ����
		{{  1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // �E��
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // �E��
		//	��
		{{ -1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // ����
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // ����
		{{  1.0f,-1.0f,-1.0f },{}, {1.0f, 1.0f}}, // �E��
		{{  1.0f,-1.0f, 1.0f },{}, {1.0f, 0.0f}}, // �E��
		//	��
		{{ -1.0f, 1.0f,-1.0f },{}, {0.0f, 1.0f}}, // ����
		{{ -1.0f, 1.0f, 1.0f },{}, {0.0f, 0.0f}}, // ����
		{{  1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // �E��
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // �E��
	};
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	indexSize = 36;
	//	�C���f�b�N�X�f�[�^
	indices =
	{
		0,1,2,
		2,1,3,
		6,5,4,
		7,5,6,
		8,9,10,
		10,9,11,
		14,13,12,
		15,13,14,
		18,17,16,
		19,17,18,
		20,21,22,
		22,21,23,
	};
	//	�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	VBInitialize(dx->GetDev(), sizeVB, vertexSize, sizeIB, &indices.front(), indexSize);

	mat.Initialize();
}

void Object3D::Initialize(MyDirectX* dx_, GPipeline* pipeline_, Shader shader)
{
	dx = dx_;
	pipeline = pipeline_;
	Initialize(shader);
}

Object3D::Object3D()
{
}

Object3D::Object3D(MyDirectX* dx_, GPipeline* pipeline_, Shader shader)
{
	dx = dx_;
	pipeline = pipeline_;
	Initialize(shader);
}


void Object3D::MatUpdate(Matrix matView, Matrix matProjection)
{
	mat.Update();

	constMapTransform->mat = mat.matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Object3D::SetVertices()
{
	Vertex* vertMap = nullptr;
	HRESULT newresult = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(newresult));

	// �S���_�ɑ΂���
	for (size_t i = 0; i < (signed)vertexSize; i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}

	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void Object3D::Draw(size_t handle)
{
	pipeline->Setting(dx->GetCmdList());
	pipeline->Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx->GetCmdList());

	//	�e�N�X�`��
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(handle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	dx->GetCmdList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}
