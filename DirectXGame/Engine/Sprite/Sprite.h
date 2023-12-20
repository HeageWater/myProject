#pragma once
#include "SpriteManager.h"
#include "VertBuff.h"
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"

/// <summary>
/// スプライトクラス
/// </summary>
class Sprite
{
public:
	Sprite();
	~Sprite();

	//初期化
	void Inilialize(SpriteCommon* spriteCommon, Matrix* matProjection_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画前準備
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画(引数の画像に)
	/// </summary>
	/// <param name="handle"></param>
	void Draw(size_t handle);

	/// <summary>
	/// 描画(元からある画像で描画)
	/// </summary>
	void Draw();

	/// <summary>
	/// 画像読み込み
	/// </summary>
	/// <param name="textureName"></param>
	void LoadFile(const wchar_t* textureName);

	/// <summary>
	/// SetColor
	/// </summary>
	void SetColor(Vector4D color);

	//スプライトコモン
	SpriteCommon* spriteCommon_ = nullptr;
	//Microsoft::WRL::ComPtr<SpriteCommon> spriteCommon_;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//リソースデスク
	D3D12_RESOURCE_DESC resDesc_{};

	//定数バッファ
	ID3D12Resource* constBuffTransform_ = nullptr;

	//コンストバッファデータマテリアル
	struct ConstBufferDataMaterial
	{
		MyMath::float4 color_;
		Matrix mat_;
	};

	//定数バッファマップ(行列用)
	ConstBufferDataMaterial* constMapTransform_ = nullptr;

public:

	//アフィン変換情報
	Vector3D scale_ = { 600,300,1 };
	Vector3D rotation_ = { 0,0,0 };
	Vector3D position_ = { 10,10,0 };

	//ワールド変換行列
	Matrix matWorld_;
	Matrix matProjection__;
public:

	//頂点データ
	Vertex vertices_[4] =
	{
		//	x,		y,		z,		u,	 v
		//前
		{{   0.0f,   0.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{   0.0f, 250.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{ 250.0f,   0.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{ 250.0f, 250.0f, -5.0f},{},{1.0f,0.0f}},//右上 
	};

	//頂点数
	uint16_t indices_[6] =
	{
		0,1,2,
		1,2,3,
	};

	//結果格納
	HRESULT result_;

private:

	//画像
	size_t tex_;
};