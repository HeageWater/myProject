#include "Object2D.h"
#include "MyMath.h"

void Object2D::SetVertices()
{
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	VertexObj* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void Object2D::SetMatScaling()
{
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
}

void Object2D::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotX;
	matRotX.m[1][1] = cos(rotAngle.x);
	matRotX.m[1][2] = sin(rotAngle.x);
	matRotX.m[2][1] = -sin(rotAngle.x);
	matRotX.m[2][2] = cos(rotAngle.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotAngle.y);
	matRotY.m[2][0] = sin(rotAngle.y);
	matRotY.m[0][2] = -sin(rotAngle.y);
	matRotY.m[2][2] = cos(rotAngle.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotAngle.z);
	matRotZ.m[0][1] = sin(rotAngle.z);
	matRotZ.m[1][0] = -sin(rotAngle.z);
	matRotZ.m[1][1] = cos(rotAngle.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
}

void Object2D::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}

Object2D::Object2D(ID3D12Device* dev, Shader shader, UINT vertexNum, float rad)
{
	HRESULT result;

#pragma region  ConstBuffer
	D3D12_HEAP_PROPERTIES heapProp{};
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
	result = dev->CreateCommittedResource(
		&cbHeapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(result));

	//	�萔�o�b�t�@�̃}�b�s���O
	result = transform->Map(0, nullptr, (void**)&constMapTransform);	//	�}�b�s���O
	assert(SUCCEEDED(result));
#pragma endregion

	vertexSize = vertexNum;
	if (vertexNum >= 3)vertexSize += 1;

	//	�C���f�b�N�X�T�C�Y

	indexSize = vertexNum * 3;
	indices.resize(indexSize);
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);

	for (size_t i = 0; i < vertexNum; i++)
	{
		size_t n = i * 3;
		indices[n + 2] = vertexSize;
		indices[n] = i;
		if (i == vertexNum - 1) {
			indices[n + 1] = 0;
		}
		else {
			indices[n + 1] = i + 1;
		}
	}

	// ���_�f�[�^�T�C�Y
	vertices.resize(vertexSize);

	float radian = MyMath::PI * 2 / (float)vertexNum;

	for (size_t i = 0; i < vertexNum; i++)
	{
		vertices[i].pos.x = rad * sin(radian * i);
		vertices[i].pos.y = rad * cos(radian * i);
		vertices[i].pos.z = 0;
	}
	vertices[vertexNum].pos.x = 0;
	vertices[vertexNum].pos.y = 0;
	vertices[vertexNum].pos.z = 0;

	for (size_t i = 0; i < vertexNum; i++)
	{
		if (i % 3 == 0) {
			vertices[i].color.x = 1.0f;
			vertices[i].color.y = 0.0f;
			vertices[i].color.z = 0.0f;
		}
		else if (i % 3 == 1) {
			vertices[i].color.x = 0.0f;
			vertices[i].color.y = 1.0f;
			vertices[i].color.z = 0.0f;
		}
		else {
			vertices[i].color.x = 0.0f;
			vertices[i].color.y = 0.0f;
			vertices[i].color.z = 1.0f;
		}
		vertices[i].color.w = 1.0f;
	}
	vertices[vertexNum].color.x = 1.0f;
	vertices[vertexNum].color.y = 1.0f;
	vertices[vertexNum].color.z = 1.0f;
	vertices[vertexNum].color.w = 1.0f;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	VBInitialize(dev, sizeVB, vertexSize, sizeIB, &indices.front(), indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	�@���x�N�g��
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	�F
	};

	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	pipeline.SetBlend(dev, GPipeline::ALPHA_BLEND);
#pragma region  WorldMatrix�����l
	scale = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle = Vector3D(0.0f, 0.0f, 0.0f);
	trans = Vector3D(0.0f, 0.0f, 0.0f);
#pragma endregion
}

void Object2D::MatUpdate(Matrix matView, Matrix matProjection)
{
#pragma region WorldMatrix
	matWorld.Identity();

	//	�X�P�[�����O
	SetMatScaling();
	matWorld *= matScale;

	//	��]
	SetMatRotation();
	matWorld *= matRot;

	//	���s�ړ�
	SetMatTransform();
	matWorld *= matTrans;
#pragma endregion

	constMapTransform->mat = matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Object2D::Draw(ID3D12GraphicsCommandList* cmdList)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(cmdList);
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}
