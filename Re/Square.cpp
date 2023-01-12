#include "Square.h"

UISquare::UISquare(ID3D12Device* dev, Shader shader)
{
	HRESULT result;

	pv[0] = { {-1.0f,-1.0f,0.1f},{0,1} };
	pv[1] = { {-1.0f, 1.0f,0.1f},{0,0} };
	pv[2] = { { 1.0f,-1.0f,0.1f},{1,1} };
	pv[3] = { { 1.0f, 1.0f,0.1f},{1,0} };
	vertexSize = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	VBInitialize(dev, sizePV, vertexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz���W
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv���W
	};
	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout));
	pipeline.SetBlend(dev, GPipeline::ALPHA_BLEND);
}

void UISquare::SetVertices()
{
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(pv[0]);

	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	ScreenVertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = pv[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
}

void UISquare::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	VertBuffUpdate(cmdList);
	//	�e�N�X�`��
	cmdList->SetGraphicsRootDescriptorTable(1, handle);
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Square::Initialize(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, int blendMord)
{
	dx = dx_;
	pipeline = pipeline_;

	HRESULT result;

#pragma region  ConstBuffer
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
	result = dx->GetDev()->CreateCommittedResource(
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

	//	�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	����
	result = dx->GetDev()->CreateCommittedResource(
		&heapProp,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(result));

	//	�萔�o�b�t�@�̃}�b�s���O
	result = material->Map(0, nullptr, (void**)&mapMaterial);	//	�}�b�s���O
	assert(SUCCEEDED(result));
#pragma endregion
	pv[0] = { {-1.0f,-1.0f,0.0f},{},{0,1} };
	pv[1] = { {-1.0f, 1.0f,0.0f},{},{0,0} };
	pv[2] = { { 1.0f,-1.0f,0.0f},{},{1,1} };
	pv[3] = { { 1.0f, 1.0f,0.0f},{},{1,0} };
	vertexSize = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	indexSize = 6;
	//	�C���f�b�N�X�f�[�^
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	VBInitialize(dx->GetDev(), sizePV, vertexSize, sizeIB, indices, indexSize);

	if (blendMord != GPipeline::NONE_BLEND) {
		pipeline->SetBlend(dx->GetDev(), blendMord);
	}
#pragma region  WorldMatrix�����l
	obj.Initialize();
#pragma endregion
}

Square::Square(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, int blendMord)
{
	Initialize(dx_, pipeline_, shader, blendMord);
}

Square::Square()
{
}

void Square::MatUpdate(Matrix matView, Matrix matProjection, Matrix billboard)
{
	obj.Update(billboard);

	constMapTransform->mat = obj.matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Square::Draw(int handle)
{
	pipeline->Setting(dx->GetCmdList());
	pipeline->Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx->GetCmdList());
	//	�e�N�X�`��
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(handle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	dx->GetCmdList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}

void Square::SetVertices()
{
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(pv[0]);

	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < vertexSize; i++) {
		vertMap[i] = pv[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
}
