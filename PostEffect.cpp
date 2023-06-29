#include "PostEffect.h"
#include "WindowApi.h"
#include "Vector3.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

PostEffect::PostEffect(ID3D12Device* device)
{
	//dxCommon_ = dxCommon;

	this->device = device;
}

PostEffect::~PostEffect()
{

}

void PostEffect::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//CD3DX12_RESOURCE_BARRIER* barrier;
	//dxCommon_->GetCommandList()

	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	cmdList->ResourceBarrier(1,
		&trans);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewPort = {};
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = WindowApi::window_width;
	viewPort.Height = WindowApi::window_height;

	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = {};
	rect.left = (long)0.0f;
	rect.top = (long)0.0f;
	rect.right = WindowApi::window_width;
	rect.bottom = WindowApi::window_height;

	cmdList->RSSetScissorRects(1, &rect);

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDraw(ID3D12GraphicsCommandList* cmdList)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	cmdList->ResourceBarrier(1, &barrier);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList, ID3D12PipelineState* pipelineState, ID3D12RootSignature* rootSignature)
{
	//���[���h���W�̍X�V
	//�萔�o�b�t�@�Ƀf�[�^�]��

	cmdList->SetPipelineState(pipelineStateP.Get());

	cmdList->SetGraphicsRootSignature(rootSignatureP.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	////�e��R�}���h���X�g

	ID3D12DescriptorHeap* srvHeap = descHeapSRV.Get();

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };

	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�[1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

}

void PostEffect::Update()
{
	//XMMATRIX matTrans;

	//matTrans = XMMatrixTranslation(position.x, position.y, 0.0f);
}

void PostEffect::Initialize()
{
	//position = { 1.0f,1.0f };

	HRESULT result;

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowApi::window_width,
		(UINT)WindowApi::window_height,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_HEAP_PROPERTIES  properities = {};
	properities.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	properities.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	properities.Type = D3D12_HEAP_TYPE_CUSTOM;

	CD3DX12_CLEAR_VALUE value = {};
	value.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	value.Color[0] = clearColor[0];
	value.Color[1] = clearColor[1];
	value.Color[2] = clearColor[2];
	value.Color[3] = clearColor[3];

	result = device->CreateCommittedResource(
		&properities,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&value,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	const UINT pixelCount = WindowApi::window_width * WindowApi::window_height;

	const UINT rowPitch = sizeof(UINT) * WindowApi::window_width;

	const UINT deathPitch = rowPitch * WindowApi::window_height;

	UINT* img = new UINT[pixelCount];

	for (size_t i = 0; i < pixelCount; i++)
	{
		img[i] = 0xff0000ff;
	}

	result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, deathPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����悤��
	srvDescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		texBuff.Get()
		, &srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc = {};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//�ݒ������SRV�p�̃f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDeac{};
	renderTargetViewDeac.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDeac.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(texBuff.Get(), &renderTargetViewDeac, descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,
			WindowApi::window_width,
			WindowApi::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_HEAP_PROPERTIES prote = {};
	prote.Type = D3D12_HEAP_TYPE_DEFAULT;

	CD3DX12_CLEAR_VALUE value2 = {};

	value2.Format = DXGI_FORMAT_D32_FLOAT;
	value2.DepthStencil.Depth = 1.0f;
	value2.DepthStencil.Stencil = 0;

	result = device->CreateCommittedResource(
		&prote,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&value2,
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));

	//dsv�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	//���_�f�[�^
	Vertex vertices[4] =
	{
		//	x,		y,		z,		u,	 v
		//�O
		{{ -0.5f, -0.5f, -0.0f},{0.0f,1.0f}},//����
		{{ -0.5f,  0.5f, -0.0f},{0.0f,0.0f}},//����
		{{  0.5f, -0.5f, -0.0f},{1.0f,1.0f}},//�E��
		{{  0.5f,  0.5f, -0.0f},{1.0f,0.0f}},//�E�� 
	};

	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	prote.Type = D3D12_HEAP_TYPE_UPLOAD;

	CD3DX12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Buffer(sizeof(Vertex));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeVB;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		//�q�[�v�ݒ�
		&prote,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));


	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeof(Vertex) * 4;

	//���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);

	//�萔�o�b�t�@
	ID3D12Resource* constBuffMaterial = nullptr;

	prote.Type = D3D12_HEAP_TYPE_UPLOAD;
	resourceDesc.Buffer((sizeof(constBuffMaterial) + 0xff) & ~0xff);

	//�q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPU�ւ̓]���p
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		//�q�[�v�ݒ�
		&prote,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	transform.mat = XMMatrixIdentity();

	//�]��
	result = constBuffMaterial->Map(0, nullptr, (void**)&this->transform);
	assert(SUCCEEDED(result));

	//�P�ʍs��
	transform.mat = XMMatrixIdentity();
	constBuffMaterial->Unmap(0, nullptr);	

	//�p�C�v���C������
	CreateGraphicsPipelineState();
}

void PostEffect::CreateGraphicsPipelineState()
{
	HRESULT result = S_FALSE;

	//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> vsBlob;

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;

	//�G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;


	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"PostEffectVS.hlsl",//�V�F�[�_�t�@�C����
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

		return;
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"PostEffectPS.hlsl",//�V�F�[�_�t�@�C����
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

		return;
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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�

	//���X�^���C�U
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//�J�����O���Ȃ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�f�v�X�X�e���V���X�e�[�g�̍쐬
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	//�����_�[�^�[�Q�b�g
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};

	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//�u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//
	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	//�`��Ώۂ͈��
	gpipeline.NumRenderTargets = 1;
	//0~255�w���RGBA
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//1�s�N�Z���̂�1��T���v�����O
	gpipeline.SampleDesc.Count = 1;

	//�f�X�N���v�^�����W�̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descriptorSRV;

	descriptorSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//���[�g�p�����^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootParams[2] = {};
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &descriptorSRV, D3D12_SHADER_VISIBILITY_ALL);

	//
	CD3DX12_STATIC_SAMPLER_DESC sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);

	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	//���[�g�V�O�l�`��
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	//���[�g�V�O�l�`���̐ݒ�
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &sampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSignatureBlob;

	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSignatureBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
		rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignatureP));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootSignatureP.Get();

	//����
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineStateP));
	assert(SUCCEEDED(result));
}
