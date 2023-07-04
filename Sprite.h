#pragma once
#include "SpriteCommon.h"
#include "Vertex.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	void Inilialize(SpriteCommon* spriteCommon);
	void Inilialize();
	void Update();
	void Draw();

	SpriteCommon* spriteCommon_ = nullptr;
	//Microsoft::WRL::ComPtr<SpriteCommon> spriteCommon_;

protected:
	////頂点データ
	//Vertex vertices[4] =
	//{
	//	//	x,		y,		z,		u,	 v
	//	//前
	//	{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
	//	{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
	//	{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//右下
	//	{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//右上 
	//};

	//Vector3 vertices[] =
	//{
	//	{-1.5f,-1.5f,0.0f},
	//	{-1.5f,+1.5f,0.0f},
	//	{+1.5f,-1.5f,0.0f}
	//};

};