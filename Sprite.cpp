#include "Sprite.h"
#include <DirectXTex.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	//delete spriteCommon_;
}

void Sprite::Inilialize(SpriteCommon* spriteCommon)
{
	//spriteCommon_ = new SpriteCommon;
	spriteCommon_ = spriteCommon;

	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

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
	result = spriteCommon->dxCommon_->GetDevice()->CreateCommittedResource(
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
	//Vector3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//�S���_�ɑ΂���
	for (auto i = 0; i < _countof(vertices); i++)
	{
		//���W�R�s�[
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
	result = spriteCommon->dxCommon_->GetDevice()->CreateCommittedResource(
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
	constMapTransform->color = XMFLOAT4(1, 1, 1, 0.5f);	//���ʂ̐F

	constMapTransform->mat = XMMatrixIdentity();

	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	UINT sizeIB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(indices));

	//�C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	result = spriteCommon_->dxCommon_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
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

void Sprite::Inilialize()
{
}

void Sprite::Update(XMMATRIX& matView)
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

	XMMATRIX matProjection = XMMatrixIdentity();

	////�ˉe�ϊ��s��
	//matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),
	//	(float)spriteCommon_->dxCommon_->GetWindow()->window_width / spriteCommon_->dxCommon_->GetWindow()->window_height,
	//	0.1f, 1000.0f);

	//���̂܂Ƃ�
	matProjection = XMMatrixOrthographicOffCenterLH
	(0,
		WindowApi::Get()->window_width,
		WindowApi::Get()->window_height,
		0,
		0.0f,
		1.0f);

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	constMapTransform->mat = this->matWorld * matProjection;

	constBuffTransform->Unmap(0, nullptr);
}

void Sprite::PreDraw()
{
	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCommandList()->SetPipelineState(spriteCommon_->pipelineState);
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->rootSignature);

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	//�O�p�`���X�g
	//spriteCommon_->dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteCommon_->dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::Draw(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//�f�X�N���v�^
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1,);

	//�摜��1�ɓ��ꂽ���̂�
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, handle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::LoadResource()
{
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WIC�e�N�X�`���̃R�[�h
	result = LoadFromWICFile(
		L"Resources/reimu.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	ScratchImage mipChain{};
	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�q���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResouceDesc{};
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;
	textureResouceDesc.Height = (UINT)metadata.height;
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//�ݒ�\����
	srvDesc.Format = resDesc.Format;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�e�N�X�`���q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//�e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff = nullptr;
	result = spriteCommon_->dxCommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//�ݒ������SRV�p�̃f�X�N���v�^�q�[�v�𐶐�
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = spriteCommon_->dxCommon_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	spriteCommon_->dxCommon_->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
}
