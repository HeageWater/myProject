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

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_RESOURCE_DESC resDesc{};

	//定数バッファ
	ID3D12Resource* constBuffTransform = nullptr;
	//定数バッファマップ(行列用)
	//ConstBufferDataMaterial* constMapTransform = nullptr;

public:
	//アフィン変換情報
	XMFLOAT3 scale = { 30,30,30 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 10,10,10 };
	//ワールド変換行列
	XMMATRIX matWorld;
public:
	//頂点データ
	//Vertex vertices[4] =
	//{
	//	//	x,		y,		z,		u,	 v
	//	//前
	//	{{ 5.0f,   5.0f, -5.0f},{},{0.0f,1.0f}},//左下
	//	{{ 5.0f,  10.0f, -5.0f},{},{0.0f,0.0f}},//左上
	//	{{ 10.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
	//	{{ 10.0f, 10.0f, -5.0f},{},{1.0f,0.0f}},//右上 
	//};

	//Vector3 vertices[] =
	//{
	//	{-1.5f,-1.5f,0.0f},
	//	{-1.5f,+1.5f,0.0f},
	//	{+1.5f,-1.5f,0.0f}
	//};

	//Vector3 vertices[4] =
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