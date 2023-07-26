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

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_RESOURCE_DESC resDesc{};

	//定数バッファ
	ID3D12Resource* constBuffTransform = nullptr;

	struct ConstBufferDataMaterial
	{
		MyMath::float4 color;
		Matrix mat;
	};

	//定数バッファマップ(行列用)
	ConstBufferDataMaterial* constMapTransform = nullptr;

public:
	//アフィン変換情報
	Vector3D scale = { -300,-300,1 };
	Vector3D rotation = { 0,0,0 };
	Vector3D position = { 10,10,0 };
	//ワールド変換行列
	Matrix matWorld;
	Matrix matProjection_;
public:
	//頂点データ
	Vertex vertices[4] =
	{
		//	x,		y,		z,		u,	 v
		//前
		{{ 250.0f,  -300.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ 250.0f,  -50.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{ 500.0f,  -300.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{ 500.0f,  -50.0f, -5.0f},{},{1.0f,0.0f}},//右上 
	};

	//Vector3D vertices[4] =
	//{
	//	{+2.5f,+2.5f,0.0f},//左下
	//	{+4.5f,+2.5f,0.0f},//右下
	//	{+2.5f,+4.5f,0.0f},//左上
	//	{+4.5f,+4.5f,0.0f},//右上
	//};

	uint16_t indices[6] =
	{
		0,1,2,
		1,2,3,
	};

	HRESULT result;

};