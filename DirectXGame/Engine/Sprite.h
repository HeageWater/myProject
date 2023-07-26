#pragma once
#include "SpriteManager.h"
#include "VertBuff.h"
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	void Inilialize(SpriteCommon* spriteCommon, Matrix* matProjection_);
	//void Update(XMMATRIX& matView);
	void Update();
	void PreDraw();
	void Draw(size_t handle);

	void TransferSpriteVertex(Vector2D size_);

	SpriteCommon* spriteCommon_ = nullptr;
	//Microsoft::WRL::ComPtr<SpriteCommon> spriteCommon_;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_RESOURCE_DESC resDesc{};

	//�萔�o�b�t�@
	ID3D12Resource* constBuffTransform = nullptr;

	struct ConstBufferDataMaterial
	{
		MyMath::float4 color;
		Matrix mat;
	};

	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataMaterial* constMapTransform = nullptr;

public:
	//�A�t�B���ϊ����
	Vector3D scale = { -300,-300,1 };
	Vector3D rotation = { 0,0,0 };
	Vector3D position = { 10,10,0 };
	//���[���h�ϊ��s��
	Matrix matWorld;
	Matrix matProjection_;
public:
	//���_�f�[�^
	Vertex vertices[4] =
	{
		//	x,		y,		z,		u,	 v
		//�O
		{{ 250.0f,  -300.0f, -5.0f},{},{0.0f,1.0f}},//����
		{{ 250.0f,  -50.0f, -5.0f},{},{0.0f,0.0f}},//����
		{{ 500.0f,  -300.0f, -5.0f},{},{1.0f,1.0f}},//�E��
		{{ 500.0f,  -50.0f, -5.0f},{},{1.0f,0.0f}},//�E�� 
	};

	//Vector3D vertices[4] =
	//{
	//	{+2.5f,+2.5f,0.0f},//����
	//	{+4.5f,+2.5f,0.0f},//�E��
	//	{+2.5f,+4.5f,0.0f},//����
	//	{+4.5f,+4.5f,0.0f},//�E��
	//};

	uint16_t indices[6] =
	{
		0,1,2,
		1,2,3,
	};

	HRESULT result;

};