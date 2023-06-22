#include "SpriteCommon.h"
#include "Vertex.h"

SpriteCommon::SpriteCommon()
{
	result = {};
}

SpriteCommon::~SpriteCommon()
{
	delete dxCommon_;
}

void SpriteCommon::Inilialize(DirectXCommon* dxCommon)
{
	////dxCommon_ = new DirectXCommon();
	////dxCommon_->Initialize();

	assert(dxCommon);

	dxCommon_ = dxCommon;

	////���_�f�[�^
	//Vertex vertices[] =
	//{
	//	//	x,		y,		z,		u,	 v
	//	//�O
	//	{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
	//	{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//����
	//	{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
	//	{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//�E�� 

	//	//��
	//	{{ -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},//����
	//	{{ -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},//����
	//	{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
	//	{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

	//	//��
	//	{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
	//	{{ -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//����
	//	{{ -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
	//	{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

	//	//�E
	//	{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
	//	{{  5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//����
	//	{{  5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
	//	{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

	//	//��
	//	{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
	//	{{ -5.0f, -5.0f, -5.0f},{},{0.0f,0.0f}},//����
	//	{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
	//	{{ -5.0f, -5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

	//	//��
	//	{{  5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},//����
	//	{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//����
	//	{{  5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
	//	{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
	//};

	Vector3 vertices[] =
	{
		{-0.5f,-0.5f,0.0f},
		{-0.5f,+0.5f,0.0f},
		{+0.5f,-0.5f,0.0f}
	};
	
	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	//UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(Vector3) * _countof(vertices));

	//���_�o�b�t�@�̐ݒ�
	//�q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPU�ւ̓]���p
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = dxCommon_->GetDevice()->CreateCommittedResource(
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
	//Vertex* vertMap = nullptr;
	Vector3* vertMap = nullptr;
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

	// ���_�o�b�t�@�r���[�̍쐬
	//D3D12_VERTEX_BUFFER_VIEW vbView{};

	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;

	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vector3);
	//vbView.StrideInBytes = sizeof(vertices[0]);

	//���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* vsBlob = nullptr;

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;

	//�G���[�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h���\�ɂ���
		"main", "vs_5_0",//�G���g���[�|�C���^���A�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h���\�ɂ���
		"main", "ps_5_0",//�G���g���[�|�C���^���A�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob, &errorBlob);

	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},

		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},

		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};


	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�

	//���X�^���C�U�̐ݒ�

	//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//�[�x�N���b�s���O��L����
	pipelineDesc.RasterizerState.DepthClipEnable = true;

	//�u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL;

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	//�`��Ώۂ͈��
	pipelineDesc.NumRenderTargets = 1;
	//0~255�w���RGBA
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//1�s�N�Z���̂�1��T���v�����O
	pipelineDesc.SampleDesc.Count = 1;


	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	//���[�g�V�O�l�`��
	//ID3D12RootSignature* rootSignature;

	result = dxCommon_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	//�p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState* pipelineState = nullptr;

	result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	verticesCount = _countof(vertices);
}

void SpriteCommon::Draw()
{
	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//dxCommon_->GetCommandList()->SetPipelineState(pipelineState);
	//dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature);

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	//�O�p�`���X�g
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//�`��R�}���h
	dxCommon_->GetCommandList()->DrawInstanced(verticesCount, 1, 0, 0);
	//dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}