#pragma once
#include "SpriteManager.h"
//#include "Vertex.h"
//#include "object.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	void Inilialize(SpriteCommon* spriteCommon);
	void Update(XMMATRIX& matView);
	void PreDraw();
	void Draw();

	void LoadResource();

	SpriteCommon* spriteCommon_ = nullptr;
	//Microsoft::WRL::ComPtr<SpriteCommon> spriteCommon_;

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_RESOURCE_DESC resDesc{};

	//�萔�o�b�t�@
	ID3D12Resource* constBuffTransform = nullptr;
	//�萔�o�b�t�@�}�b�v(�s��p)
	//ConstBufferDataMaterial* constMapTransform = nullptr;

public:
	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 30,30,30 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 10,10,10 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
public:
	//���_�f�[�^
	//Vertex vertices[4] =
	//{
	//	//	x,		y,		z,		u,	 v
	//	//�O
	//	{{ 5.0f,   5.0f, -5.0f},{},{0.0f,1.0f}},//����
	//	{{ 5.0f,  10.0f, -5.0f},{},{0.0f,0.0f}},//����
	//	{{ 10.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//�E��
	//	{{ 10.0f, 10.0f, -5.0f},{},{1.0f,0.0f}},//�E�� 
	//};

	//Vector3 vertices[] =
	//{
	//	{-1.5f,-1.5f,0.0f},
	//	{-1.5f,+1.5f,0.0f},
	//	{+1.5f,-1.5f,0.0f}
	//};

	//Vector3 vertices[4] =
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