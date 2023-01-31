//#include "SpriteCommon.h"
//
//SpriteCommon::SpriteCommon()
//{
//}
//
//SpriteCommon::~SpriteCommon()
//{
//}
//
//void SpriteCommon::Inilialize(DirectXCommon* dxCommon)
//{
//	dxCommon = new DirectXCommon();
//	dxCommon->Initialize();
//
//	assert(dxCommon);
//
//	this->dxCommon = *dxCommon;
//
//	//���_�f�[�^�\����
//	struct Vertex
//	{
//		XMFLOAT3 pos;
//		XMFLOAT3 noemal;
//		XMFLOAT2 uv;
//	};
//
//	//���_�f�[�^
//	Vertex vertices[] =
//	{
//		//	x,		y,		z,		u,	 v
//		//�O
//		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
//		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//����
//		{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
//		{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//�E�� 
//
//		//��
//		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},//����
//		{{ -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},//����
//		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
//		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
//
//		//��
//		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
//		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//����
//		{{ -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
//		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
//
//		//�E
//		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
//		{{  5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//����
//		{{  5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
//		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
//
//		//��
//		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
//		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,0.0f}},//����
//		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
//		{{ -5.0f, -5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
//
//		//��
//		{{  5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},//����
//		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//����
//		{{  5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
//		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
//	};
//
//	//�C���f�b�N�X�f�[�^
//	unsigned short indices[] =
//	{
//		//�O
//		0,1,2,
//		2,1,3,
//		//��
//		6,5,4,
//		7,5,6,
//		//��
//		8,9,10,
//		11,10,9,
//		//�E
//		14,13,12,
//		13,14,15,
//		//��
//		18,17,16,
//		17,18,19,
//		//��
//		20,21,22,
//		23,22,21,
//	};
//
//	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
//
//	//���_�o�b�t�@�̐ݒ�
//	//�q�[�v�̐ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{};
//
//	//GPU�ւ̓]���p
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC resDesc{};
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB;
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//���_�o�b�t�@�̐���
//	ID3D12Resource* vertBuff = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		//�q�[�v�ݒ�
//		&heapProp,
//		D3D12_HEAP_FLAG_NONE,
//
//		//���\�[�X�ݒ�
//		&resDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//gpu��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
//	Vertex* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//
//	//�S���_�ɑ΂���
//	for (int i = 0; i < _countof(vertices); i++)
//	{
//		//���W�R�s�[
//		vertMap[i] = vertices[i];
//	}
//
//	//�Ȃ�����폜
//	vertBuff->Unmap(0, nullptr);
//
//	//���_�o�b�t�@�r���[�̍쐬
//	//D3D12_VERTEX_BUFFER_VIEW vbView{};
//
//	//GPU���z�A�h���X
//	vbView->BufferLocation = vertBuff->GetGPUVirtualAddress();
//
//	//���_�o�b�t�@�̃T�C�Y
//	vbView->SizeInBytes = sizeVB;
//
//	//���_1���̃f�[�^�T�C�Y
//	//vbView.StrideInBytes = sizeof(XMFLOAT3);
//	vbView->StrideInBytes = sizeof(vertices[0]);
//
//	//���_�V�F�[�_�I�u�W�F�N�g
//	ID3DBlob* vsBlob = nullptr;
//
//	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
//	ID3DBlob* psBlob = nullptr;
//
//	//�G���[�I�u�W�F�N�g
//	ID3DBlob* errorBlob = nullptr;
//
//	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
//	result = D3DCompileFromFile(
//		L"BasicVS.hlsl",//�V�F�[�_�t�@�C����
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h���\�ɂ���
//		"main", "vs_5_0",//�G���g���[�|�C���^���A�V�F�[�_���f���w��
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
//		0,
//		&vsBlob, &errorBlob);
//
//	//�G���[�Ȃ�
//	if (FAILED(result)) {
//		//errorBlob����G���[���e��string�^�ɃR�s�[
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		//�G���[���e���o�̓E�B���h�E�ɕ\��
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
//	result = D3DCompileFromFile(
//		L"BasicPS.hlsl",//�V�F�[�_�t�@�C����
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h���\�ɂ���
//		"main", "ps_5_0",//�G���g���[�|�C���^���A�V�F�[�_���f���w��
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
//		0,
//		&psBlob, &errorBlob);
//
//	//�G���[�Ȃ�
//	if (FAILED(result)) {
//		//errorBlob����G���[���e��string�^�ɃR�s�[
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		//�G���[���e���o�̓E�B���h�E�ɕ\��
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//	//���_���C�A�E�g
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
//	{
//		{
//			"POSITION",
//			0,
//			DXGI_FORMAT_R32G32B32_FLOAT,
//			0,
//			D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//			0
//		},
//
//		{
//			"NORMAL",
//			0,
//			DXGI_FORMAT_R32G32B32_FLOAT,
//			0,
//			D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//			0
//		},
//
//		{
//			"TEXCOORD",
//			0,
//			DXGI_FORMAT_R32G32_FLOAT,
//			0,
//			D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//			0
//		},
//	};
//
//	//�O���t�B�b�N�X�p�C�v���C���ݒ�
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
//
//	//�V�F�[�_�[�̐ݒ�
//	gpipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//	gpipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//	gpipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//	gpipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//
//	//�T���v���}�X�N�̐ݒ�
//	gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
//
//	//���X�^���C�U�̐ݒ�
//
//	//�J�����O���Ȃ�
//	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
//	//�|���S�����h��Ԃ�
//	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
//	//�[�x�N���b�s���O��L����
//	gpipelineDesc.RasterizerState.DepthClipEnable = true;
//
//	//�u�����h�X�e�[�g
//	//gpipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
//	//	= D3D12_COLOR_WRITE_ENABLE_ALL;
//	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
//	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipelineDesc.BlendState.RenderTarget[0];
//	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//
//	//�u�����h�𖳌�
//	blenddesc.BlendEnable = false;
//
//	//�f�v�X�X�e���V���X�e�[�g�̍쐬
//	gpipelineDesc.DepthStencilState.DepthEnable = true;
//	gpipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
//	gpipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
//	gpipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
//
//	//RBGA�S�Ẵ`�����l����`��
//	//�|���S�����h��Ԃ�
//	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
//
//	//���C���[�t���[��
//	//gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
//
//	//���_���C�A�E�g�̐ݒ�
//	gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
//	gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);
//
//	//�}�`�̌`��ݒ�
//	gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	//���̑��̐ݒ�
//	//�`��Ώۂ͈��
//	gpipelineDesc.NumRenderTargets = 1;
//	//0~255�w���RGBA
//	gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	//1�s�N�Z���̂�1��T���v�����O
//	gpipelineDesc.SampleDesc.Count = 1;
//
//	//�q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES cbHeapProp{};
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC cbResouceDesc{};
//	cbResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResouceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
//	cbResouceDesc.Height = 1;
//	cbResouceDesc.DepthOrArraySize = 1;
//	cbResouceDesc.MipLevels = 1;
//	cbResouceDesc.SampleDesc.Count = 1;
//	cbResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	ID3D12Resource* constBuffMaterial = nullptr;
//	//�萔�o�b�t�@�̐���
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&cbHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&cbResouceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffMaterial));
//	assert(SUCCEEDED(result));
//
//	//�萔�o�b�t�@�̃}�b�s���O
//	ConstBufferDataMaterial* constMapMaterial = nullptr;
//	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
//	assert(SUCCEEDED(result));
//
//	//�l���������ނƎ����I�ɓ]������� 
//	constMapMaterial->color = XMFLOAT4(1, 1, 1, 0.5f);	//���ʂ̐F
//
//	//�f�X�N���v�^�����W�̐ݒ�
//	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
//	descriptorRange.NumDescriptors = 1;
//	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
//	descriptorRange.BaseShaderRegister = 0;
//	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//	//���[�g�p�����^�̐ݒ�
//	D3D12_ROOT_PARAMETER rootParams[3] = {};
//
//	//�萔�o�b�t�@0��
//	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
//	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
//	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
//	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
//	//�e�N�X�`�����W�X�^0��
//	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//�萔�o�b�t�@�r���[
//	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//�萔�o�b�t�@�ԍ�
//	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�t�H���g�l
//	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
//	// �萔�o�b�t�@1��
//	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
//	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
//	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
//	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
//
//	//�e�N�X�`���T���v���[�̐ݒ�
//	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
//	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
//	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
//	samplerDesc.MinLOD = 0.0f;
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
//	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//
//	//���[�g�V�O�l�`��
//	ID3D12RootSignature* rootSignature;
//	//���[�g�V�O�l�`���̐ݒ�
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//	rootSignatureDesc.pParameters = rootParams;
//	rootSignatureDesc.NumParameters = _countof(rootParams);
//	rootSignatureDesc.pStaticSamplers = &samplerDesc;
//	rootSignatureDesc.NumStaticSamplers = 1;
//
//	//���[�g�V�O�l�`���̃V���A���C�Y
//	ID3DBlob* rootSigBlob = nullptr;
//	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
//		&rootSigBlob, &errorBlob);
//	assert(SUCCEEDED(result));
//
//	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
//		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
//	assert(SUCCEEDED(result));
//	rootSigBlob->Release();
//	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
//	gpipelineDesc.pRootSignature = rootSignature;
//
//	//�p�C�v�����X�e�[�g�̐���
//	ID3D12PipelineState* pipelineState = nullptr;
//	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
//	assert(SUCCEEDED(result));
//
//	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
//	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
//
//	////���\�[�X�ݒ�
//	//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	//resDesc.Width = sizeIB;
//	//resDesc.Height = 1;
//	//resDesc.DepthOrArraySize = 1;
//	//resDesc.MipLevels = 1;
//	//resDesc.SampleDesc.Count = 1;
//	//resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//�C���f�b�N�X�o�b�t�@�̐���
//	ID3D12Resource* indexBuff = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&heapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff));
//
//	//�C���f�b�N�X�o�b�t�@���}�b�s���O
//	uint16_t* indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
//	//�S�C���f�b�N�X�ɑ΂���
//	for (int i = 0; i < _countof(indices); i++)
//	{
//		//�C���f�b�N�X���R�s�[
//		indexMap[i] = indices[i];
//	}
//	//�}�b�s���O����
//	indexBuff->Unmap(0, nullptr);
//
//	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
//	D3D12_INDEX_BUFFER_VIEW ibView{};
//	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibView.Format = DXGI_FORMAT_R16_UINT;
//	ibView.SizeInBytes = sizeIB;
//
//	//��������
//
//	//1��
//	TexMetadata metadata{};
//	ScratchImage scratchImg{};
//	//WIC�e�N�X�`���̃R�[�h
//	result = LoadFromWICFile(
//		L"Resources/cube.jpg",
//		WIC_FLAGS_NONE,
//		&metadata,
//		scratchImg
//	);
//
//	ScratchImage mipChain{};
//	//�~�b�v�}�b�v����
//	result = GenerateMipMaps(
//		scratchImg.GetImages(),
//		scratchImg.GetImageCount(),
//		scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT,
//		0,
//		mipChain);
//	if (SUCCEEDED(result))
//	{
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//
//	//�ǂݍ��񂾃f�B�q���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
//	metadata.format = MakeSRGB(metadata.format);
//
//	//�e�N�X�`���q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty =
//		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC textureResouceDesc{};
//	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResouceDesc.Format = metadata.format;
//	textureResouceDesc.Width = metadata.width;
//	textureResouceDesc.Height = (UINT)metadata.height;
//	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
//	textureResouceDesc.SampleDesc.Count = 1;
//
//	//�e�N�X�`���o�b�t�@�̐���
//	ID3D12Resource* texBuff = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResouceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff));
//
//	//�S�~�b�v�}�b�v�ɂ���
//	for (size_t i = 0; i < metadata.mipLevels; i++)
//	{
//		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
//		const Image* img = scratchImg.GetImage(i, 0, 0);
//		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
//		result = texBuff->WriteToSubresource(
//			(UINT)i,
//			nullptr,
//			img->pixels,
//			(UINT)img->rowPitch,
//			(UINT)img->slicePitch);
//		assert(SUCCEEDED(result));
//	}
//
//	//2��
//	TexMetadata metadata2{};
//	ScratchImage scratchImg2{};
//	//WIC�e�N�X�`���̃R�[�h
//	result = LoadFromWICFile(
//		L"Resources/reimu.png",
//		WIC_FLAGS_NONE,
//		&metadata2,
//		scratchImg2
//	);
//
//	ScratchImage mipChain2{};
//	//�~�b�v�}�b�v����
//	result = GenerateMipMaps(
//		scratchImg2.GetImages(),
//		scratchImg2.GetImageCount(),
//		scratchImg2.GetMetadata(),
//		TEX_FILTER_DEFAULT,
//		0,
//		mipChain2);
//	if (SUCCEEDED(result))
//	{
//		scratchImg2 = std::move(mipChain2);
//		metadata2 = scratchImg2.GetMetadata();
//	}
//
//	//�ǂݍ��񂾃f�B�q���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
//	metadata2.format = MakeSRGB(metadata2.format);
//
//	//���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC textureResouceDesc2{};
//	textureResouceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResouceDesc2.Format = metadata2.format;
//	textureResouceDesc2.Width = metadata2.width;
//	textureResouceDesc2.Height = (UINT)metadata2.height;
//	textureResouceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
//	textureResouceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
//	textureResouceDesc2.SampleDesc.Count = 1;
//
//	//�e�N�X�`���o�b�t�@�̐���
//	ID3D12Resource* texBuff2 = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResouceDesc2,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff2));
//}
//
//void SpriteCommon::Draw()
//{
//	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
//	dxCommon.GetCommandList()->SetPipelineState(pipelineState);
//	dxCommon.GetCommandList()->SetGraphicsRootSignature(rootSignature);
//
//	//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
//	dxCommon.GetCommandList()->IASetIndexBuffer(&ibView);
//
//	//�v���~�e�B�u�`��̐ݒ�R�}���h
//	//�O�p�`���X�g
//	dxCommon.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
//	dxCommon.GetCommandList()->IASetVertexBuffers(0, 1, vbView.Get());
//
//	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
//	dxCommon.GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
//
//	//SRV�q�[�v�̐ݒ�R�}���h
//	dxCommon.GetCommandList()->SetDescriptorHeaps(1, &srvHeap);
//
//	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���)
//	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
//
//	//�`��t���O
//	/*if (draw_flg)
//	{
//		srvGpuHandle.ptr += incrementSize;
//	}*/
//
//	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
//	dxCommon.GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
//
//	//�`��R�}���h
//	//object3ds->DrawObject3d(dxCommon.GetCommandList(), vbView, ibView, _countof(indices));
//}
