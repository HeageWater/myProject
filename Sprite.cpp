#include "Sprite.h"

Sprite::Sprite()
{
	//spriteCommon->dxCommon();

	//	dxCommon = new DirectXCommon();
	//dxCommon->Initialize();

	//assert(dxCommon);

	//this->dxCommon = *dxCommon;

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

	//頂点バッファの設定
	//ヒープの設定
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPUへの転送用
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	////頂点バッファの生成
	//ID3D12Resource* vertBuff = nullptr;
	//result = spriteCommon->dxCommon->GetDevice()->CreateCommittedResource(
	//	//ヒープ設定
	//	&heapProp,
	//	D3D12_HEAP_FLAG_NONE,

	//	//リソース設定
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
