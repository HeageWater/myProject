#include "object.h"
#include "key.h"
#include "WindowApi.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Controller.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Sound.h"
#include "ObjFile.h"
#include "PostEffect.h"
#include <fstream>
#include "ImguiManager.h"
#include <imgui.h>
#include <wrl.h>

using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//3D�I�u�W�F�N�g�̐�
	const size_t kObjectCount = 1;

	//�r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, 10);		//���_���W
	XMFLOAT3 target(0, 0, 0);		//���ӓ_���W
	XMFLOAT3 up(0, 1, 0);			//������x�N�g��
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//�ˉe�ϊ��s��
	XMMATRIX matProjection;

	//�J�����̉�]�p
	float angle = 0.0f;

	bool draw_flg = false;

	MSG msg{};

	HRESULT result;

	//�錾
	//ComPtr<DirectXCommon> dxCommon = nullptr;
	DirectXCommon* dxCommon = new DirectXCommon();
	dxCommon->Initialize();

	//ComPtr<Key> key = new Key(dxCommon->GetWindow()->GetHInstance(), dxCommon->GetWindow()->GetHwnd());
	Key* key = new Key(dxCommon->GetWindow()->GetHInstance(), dxCommon->GetWindow()->GetHwnd());
	key->Initialize();

	/*ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;*/

	//ComPtr<Controller> controller = nullptr;
	Controller* controller = nullptr;
	controller = Controller::GetInstance();

	//Sound* sound = new Sound();

	//Sound sound2;

	//int bgm = sound->SoundLoadWave("Resource/BGM.wav");
	//int bgm = sound2.SoundLoadWave("Resource//BGM.wav");



	//������

	//DirectX���������������܂�

	//�`�揉����������������

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 noemal;
		XMFLOAT2 uv;
	};

	//���_�f�[�^
	Vertex vertices[] =
	{
		//	x,		y,		z,		u,	 v
		//�O
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//����
		{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//�E�� 

		//��
		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},//����
		{{ -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},//����
		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

		//��
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//����
		{{ -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

		//�E
		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
		{{  5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//����
		{{  5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

		//��
		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//����
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,0.0f}},//����
		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
		{{ -5.0f, -5.0f,  5.0f},{},{1.0f,0.0f}},//�E��

		//��
		{{  5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},//����
		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//����
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},//�E��
		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//�E��
	};

	//�C���f�b�N�X�f�[�^
	unsigned short indices[] =
	{
		//�O
		0,1,2,
		2,1,3,
		//��
		6,5,4,
		7,5,6,
		//��
		8,9,10,
		11,10,9,
		//�E
		14,13,12,
		13,14,15,
		//��
		18,17,16,
		17,18,19,
		//��
		20,21,22,
		23,22,21,
	};

	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//�@���v�Z
	for (int i = 0; i < sizeof(indices) / sizeof(unsigned short) / 3; i++)
	{
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0��p1�x�N�g��,p0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//���K��(�������P��)
		normal = XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].noemal, normal);
		XMStoreFloat3(&vertices[index1].noemal, normal);
		XMStoreFloat3(&vertices[index2].noemal, normal);
	}

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
	result = dxCommon->GetDevice()->CreateCommittedResource(
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
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		//���W�R�s�[
		vertMap[i] = vertices[i];
	}

	//�Ȃ�����폜
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;

	//���_1���̃f�[�^�T�C�Y
	//vbView.StrideInBytes = sizeof(XMFLOAT3);
	vbView.StrideInBytes = sizeof(vertices[0]);

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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	gpipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�

	//���X�^���C�U�̐ݒ�

	//�J�����O���Ȃ�
	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//�|���S�����h��Ԃ�
	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//�[�x�N���b�s���O��L����
	gpipelineDesc.RasterizerState.DepthClipEnable = true;

	//�u�����h�X�e�[�g
	//gpipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipelineDesc.BlendState.RenderTarget[0];
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

	//�f�v�X�X�e���V���X�e�[�g�̍쐬
	gpipelineDesc.DepthStencilState.DepthEnable = true;
	gpipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//RBGA�S�Ẵ`�����l����`��
	//�|���S�����h��Ԃ�
	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;

	//���C���[�t���[��
	//gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	//���_���C�A�E�g�̐ݒ�
	gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	//�`��Ώۂ͈��
	gpipelineDesc.NumRenderTargets = 1;
	//0~255�w���RGBA
	gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//1�s�N�Z���̂�1��T���v�����O
	gpipelineDesc.SampleDesc.Count = 1;


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

	ID3D12Resource* constBuffMaterial = nullptr;
	//�萔�o�b�t�@�̐���
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	//�l���������ނƎ����I�ɓ]������� 
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 0.5f);	//���ʂ̐F

	constMapMaterial->mat = XMMatrixIdentity();

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	//�萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//�萔�o�b�t�@�r���[
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//�萔�o�b�t�@�ԍ�
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����
	// �萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//�萔�o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_�[���猩����

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

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
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

	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipelineDesc.pRootSignature = rootSignature;

	//�p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	////���\�[�X�ݒ�
	//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//resDesc.Width = sizeIB;
	//resDesc.Height = 1;
	//resDesc.DepthOrArraySize = 1;
	//resDesc.MipLevels = 1;
	//resDesc.SampleDesc.Count = 1;
	//resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
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

	//��������

	//1��
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WIC�e�N�X�`���̃R�[�h
	result = LoadFromWICFile(
		L"Resources/cube.jpg",
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

	//�e�N�X�`���q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResouceDesc{};
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;
	textureResouceDesc.Height = (UINT)metadata.height;
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
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


	//2��
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	//WIC�e�N�X�`���̃R�[�h
	result = LoadFromWICFile(
		L"Resources/reimu.png",
		WIC_FLAGS_NONE,
		&metadata2,
		scratchImg2
	);

	ScratchImage mipChain2{};
	//�~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg2.GetImages(),
		scratchImg2.GetImageCount(),
		scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain2);
	if (SUCCEEDED(result))
	{
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�q���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata2.format = MakeSRGB(metadata2.format);

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResouceDesc2{};
	textureResouceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc2.Format = metadata2.format;
	textureResouceDesc2.Width = metadata2.width;
	textureResouceDesc2.Height = (UINT)metadata2.height;
	textureResouceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResouceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResouceDesc2.SampleDesc.Count = 1;

	//�e�N�X�`���o�b�t�@�̐���
	ID3D12Resource* texBuff2 = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResouceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));


	//�ˉe�ϊ�
	{
		////�q�[�v�ݒ�
		//D3D12_HEAP_PROPERTIES cbHeapProp{};
		//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		////���\�[�X�ݒ�	
		//D3D12_RESOURCE_DESC cbResouceDesc{};
		//cbResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		//cbResouceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		//cbResouceDesc.Height = 1;
		//cbResouceDesc.DepthOrArraySize = 1;
		//cbResouceDesc.MipLevels = 1;
		//cbResouceDesc.SampleDesc.Count = 1;
		//cbResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		////�萔�o�b�t�@����
		//result = device->CreateCommittedResource(
		//	&cbHeapProp,
		//	D3D12_HEAP_FLAG_NONE,
		//	&cbResouceDesc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffTransform0));
		//assert(SUCCEEDED(result));

		//result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);
		//assert(SUCCEEDED(result));

		////�萔�o�b�t�@����Part2
		//result = device->CreateCommittedResource(
		//	&cbHeapProp,
		//	D3D12_HEAP_FLAG_NONE,
		//	&cbResouceDesc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffTransform1));
		//assert(SUCCEEDED(result));

		//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);
		//assert(SUCCEEDED(result));

		////���s���e�s��̌v�Z
		//constMapTransform->mat = XMMatrixIdentity();

		//constMapTransform->mat.r[0].m128_f\[0] = 2.0f / window->window_width;
		//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / window->window_height;
		//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
		//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

		////���̂܂Ƃ�
		//XMMATRIX newb = XMMatrixOrthographicOffCenterLH
		//(0, window->window_width,
		//	window->window_height,0,
		//	0.0f, 1.0f);

		/*constMapTransform->mat = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)window->window_width / window->window_height,
			0.1f, 1000.0f*/

			//�ˉe�ϊ��s��
		matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)dxCommon->GetWindow()->window_width / dxCommon->GetWindow()->window_height,
			0.1f, 1000.0f);

		//matProjection = XMMatrixIdentity();
	}

	//device�ɑ�����Ă�������
	//ComPtr<Object3ds> object3ds = new Object3ds(dxCommon->GetDevice());
	Object3ds* object3ds = new Object3ds(dxCommon->GetDevice());
	Object3ds* object3ds2 = new Object3ds(dxCommon->GetDevice());

	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg2.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff2->WriteToSubresource(
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
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//�e�N�X�`���o�b�t�@��艺�ɏ���
	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//�ݒ�\����
	srvDesc.Format = resDesc.Format;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dxCommon->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	//�n���h���̃T�C�Y����
	UINT incrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incrementSize;

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};			//�ݒ�\����
	srvDesc2.Format = textureResouceDesc2.Format;	//RGBA float
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc2.Texture2D.MipLevels = textureResouceDesc2.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dxCommon->GetDevice()->CreateShaderResourceView(texBuff2, &srvDesc2, srvHandle);

	//�T�E���h�ݒ�
	//sound2.SoundPlayWave(bgm);
	//sound->SoundPlayLoopWave(bgm);

	//�`�揉�������������܂�

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Inilialize(dxCommon);

	Sprite* sprite = new Sprite();
	sprite->Inilialize(spriteCommon);

	sprite->LoadResource();

	PostEffect* postEffect = nullptr;

	postEffect = new PostEffect(dxCommon->GetDevice());
	postEffect->Initialize();

	ImguiManager* imguiManager = new ImguiManager();
	imguiManager->Initialize(dxCommon);

	//�Q�[�����[�v1
	while (true)
	{
		//���t���[��������������

		//�L�[���͍X�V
		key->Update();
		controller->Update();

		//postEffect->Update();

		//windows�̃��b�Z�[�W����
		if (dxCommon->GetWindow()->ProcessMessege())
		{
			break;
		}

		//�`��t���O�؂�ւ�
		if (key->Push(DIK_SPACE))
		{
			draw_flg = !draw_flg;
		}

		//�ړ����x
		float speed = 0.4f;

		Vector2 vec = { 0,0 };
		vec = controller->GetLeftStickVec();

		float DeadSpace = 0.3f;

		//�ړ�
		if (key->Keep(DIK_UP) || vec.y < -DeadSpace)
		{
			eye.z -= speed;
		}
		if (key->Keep(DIK_DOWN) || vec.y > DeadSpace)
		{
			eye.z += speed;
		}
		if (key->Keep(DIK_RIGHT) || vec.x > DeadSpace)
		{
			object3ds->position.x += speed;
		}
		if (key->Keep(DIK_LEFT) || vec.x < -DeadSpace)
		{
			object3ds->position.x -= speed;
		}

		if (key->Keep(DIK_ESCAPE))
		{
			break;
		}

		if (key->Keep(DIK_D))
		{
			eye.x += 3;		//���_���W
		}
		if (key->Keep(DIK_A))
		{
			eye.x -= 3;		//���_���W
		}
		if (key->Keep(DIK_W))
		{
			eye.y += 3;		//���_���W
		}
		if (key->Keep(DIK_S))
		{
			eye.y -= 3;		//���_���W
		}
		if (key->Keep(DIK_Q))
		{
			eye.z += 3;		//���_���W
		}
		if (key->Keep(DIK_E))
		{
			eye.z -= 3;		//���_���W
		}

		matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		//�X�V����
		object3ds->UpdateObject3d(matView, matProjection);
		sprite->Update(matView);

		postEffect->Update(matView, matProjection);

		//ImGui��������
		imguiManager->Begin();

		ImGui::InputFloat("object3ds->position.x", &object3ds->position.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("sprite->position.x", &sprite->position.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("sprite->position.y", &sprite->position.y, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("sprite->position.z", &sprite->position.z, 0.0f, 10.0f, "%f");

		ImGui::InputFloat("eye->scale.x", &eye.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("eye->scale.y", &eye.y, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("eye->scale.z", &eye.z, 0.0f, 10.0f, "%f");

		ImGui::InputFloat("post->rotation.x", &postEffect->rotation.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("post->rotation.y", &postEffect->rotation.y, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("post->rotation.z", &postEffect->rotation.z, 0.0f, 10.0f, "%f");
		//ImGui::Text("Hello, world %d", 123);

		//ImGui�����܂�
		imguiManager->End();

		//���t���[�����������܂�


		//2.�`���̕ύX

		//3.��ʃN���A

		//4.�`�揈����������

		//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dxCommon->GetCommandList()->SetPipelineState(pipelineState);
		dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature);

		//�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
		dxCommon->GetCommandList()->IASetIndexBuffer(&ibView);

		//�v���~�e�B�u�`��̐ݒ�R�}���h
		//�O�p�`���X�g
		dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//���_�o�b�t�@�r���[�̐ݒ�R�}���h
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		//SRV�q�[�v�̐ݒ�R�}���h
		dxCommon->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);

		//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//DirectXCommon
		dxCommon->PreDraw();

		//�`��t���O
		if (draw_flg)
		{
			//srvGpuHandle.ptr += incrementSize;

			//PostEffect����
			postEffect->PreDraw(dxCommon->GetCommandList());

			//�`��R�}���h
			object3ds->DrawObject3d(dxCommon->GetCommandList(), vbView, ibView, _countof(indices));

			//Imgui�`��
			imguiManager->Draw(dxCommon);

			//PostEffect�I��
			postEffect->PostDraw(dxCommon->GetCommandList());

			//PostEffect�`��
			postEffect->Draw(dxCommon->GetCommandList(), pipelineState, rootSignature, ibView);
		}
		else
		{
			//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
			//���̒��ŉ摜����Ă邩�炳�����ƃX�v���C�g�Ɉړ�
			//dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//�`��R�}���h
			object3ds->DrawObject3d(dxCommon->GetCommandList(), vbView, ibView, _countof(indices));

			//postEffect->Draw();
			sprite->PreDraw();

			//�摜��1�ɓ��ꂽ���̂�
			dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//�O�p�`�`��
			sprite->Draw(srvGpuHandle);

			//Imgui�`��
			imguiManager->Draw(dxCommon);
		}
		//4.�`�揈�������܂�

		//5.���\�[�X�o���A
		dxCommon->PostDraw();

		////�`���Ԃ���
	}

	//windowAPI��n��

	//�E�B���h�E�N���X��o�^����
	dxCommon->GetWindow()->Finalize();
	imguiManager->Finalize();

	//���f�[�^���
	delete key;
	delete object3ds;
	delete dxCommon;
	//delete spriteCommon;
	//delete sprite;
	delete postEffect;
	delete imguiManager;

	return 0;
}