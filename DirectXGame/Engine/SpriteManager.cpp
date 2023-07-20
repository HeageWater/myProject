#include "SpriteManager.h"
using namespace DirectX;

#pragma comment(lib,"d3dcompiler.lib")

SpriteCommon::SpriteCommon()
{
	result = {};
}

SpriteCommon::~SpriteCommon()
{
	//delete dxCommon_;
}

void SpriteCommon::Inilialize(MyDirectX* dxCommon)
{
	assert(dxCommon);

	dxCommon_ = dxCommon;

	//���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* vsBlob = nullptr;

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;

	//�G���[�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl",//�V�F�[�_�t�@�C����
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
		L"SpritePS.hlsl",//�V�F�[�_�t�@�C����
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
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//�[�x�N���b�s���O��L����
	pipelineDesc.RasterizerState.DepthClipEnable = false;



	//�u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];

	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//�u�����h�𖳌�
	blenddesc.BlendEnable = false;

	{
		////�u�����h��L��
		//blenddesc.BlendEnable = true;
		////���Z
		//blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		////�\�[�X�̒l��10%�g��
		//blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		////�f�X�g�̒l��0%�g��
		//blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		//���Z����
		////���Z
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		////�\�[�X�̒l��10%�g��
		//blenddesc.SrcBlend = D3D12_BLEND_ONE;
		////�f�X�g�̒l��0%�g��
		//blenddesc.DestBlend = D3D12_BLEND_ZERO;

		////���Z
		//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		////�\�[�X�̒l��10%�g��
		//blenddesc.SrcBlend = D3D12_BLEND_ONE;
		////�f�X�g�̒l��0%�g��
		//blenddesc.DestBlend = D3D12_BLEND_ONE;

		////�F���]
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		////1.0f-�f�X�g�J���[�̒l
		//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		////�g��Ȃ�
		//blenddesc.DestBlend = D3D12_BLEND_ZERO;

		////����������
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		////�\�[�X�̃��l
		//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		////1.0f-�\�[�X�̃��l
		//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}


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



	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.RegisterSpace = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[2] = {};

	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//�萔�o�b�t�@�r���[
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//�萔�o�b�t�@�ԍ�
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//// �萔�o�b�t�@1��
	//rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	//rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	//rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	//rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����

	//�e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	//���[�g�V�O�l�`��
	//ID3D12RootSignature* rootSignature;

	result = dxCommon_->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	//�p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState* pipelineState = nullptr;

	result = dxCommon_->GetDev()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//verticesCount = _countof(vertices);
}

void SpriteCommon::Draw()
{
	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	dxCommon_->GetCmdList()->SetPipelineState(pipelineState);
	dxCommon_->GetCmdList()->SetGraphicsRootSignature(rootSignature);

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	//�O�p�`���X�g
	dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	//�`��R�}���h
	dxCommon_->GetCmdList()->DrawInstanced(verticesCount, 1, 0, 0);
	//dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}