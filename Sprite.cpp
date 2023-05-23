#include "Sprite.h"

Sprite::Sprite()
{
	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 noemal;
		XMFLOAT2 uv;
	};

	//頂点データ
	Vertex vertices[] =
	{
		//	x,		y,		z,		u,	 v
		//前
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//右上 

		//後
		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//左
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
		{{ -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//右
		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{  5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//下
		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
		{{ -5.0f, -5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//上
		{{  5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},//右下
		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上
	};

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
}

Sprite::~Sprite()
{
}

void Sprite::Inilialize(SpriteCommon* spriteCommon)
{
	this->spriteCommon = new SpriteCommon;
	this->spriteCommon = spriteCommon;
}
