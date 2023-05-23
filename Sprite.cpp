#include "Sprite.h"

Sprite::Sprite()
{
	//spriteCommon->dxCommon();

	//	dxCommon = new DirectXCommon();
	//dxCommon->Initialize();

	//assert(dxCommon);

	//this->dxCommon = *dxCommon;

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

	//���_�f�[�^�T�C�Y�@= ���_�f�[�^�T�C�Y��� * �v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

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

	////���_�o�b�t�@�̐���
	//ID3D12Resource* vertBuff = nullptr;
	//result = spriteCommon->dxCommon->GetDevice()->CreateCommittedResource(
	//	//�q�[�v�ݒ�
	//	&heapProp,
	//	D3D12_HEAP_FLAG_NONE,

	//	//���\�[�X�ݒ�
	//	&resDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff));
	//assert(SUCCEEDED(result));
}

Sprite::~Sprite()
{
}

void Sprite::Inilialize(SpriteCommon* spriteCommon)
{
	this->spriteCommon = new SpriteCommon;
	this->spriteCommon = spriteCommon;
}
