#include "Sprite.h"
#include <DirectXTex.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	//delete spriteCommon_;
}

void Sprite::Inilialize(SpriteCommon* spriteCommon, Matrix* matProjection_)
{
	//spriteCommon_ = new SpriteCommon;
	this->spriteCommon_ = spriteCommon;
	this->matProjection_ = *matProjection_;

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//UINT sizeVB = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//VBInitialize(dx->GetDev(), sizePV, vertexSize);
	//頂点バッファの設定
	//ヒープの設定
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPUへの転送用
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	//D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	//resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = spriteCommon->dxCommon_->GetDev()->CreateCommittedResource(
		//ヒープ設定
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		//リソース設定
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//gpu状のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	//ScreenVertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//全頂点に対して
	//for (size_t i = 0; i < vertexSize; i++)
	for (size_t i = 0; i < _countof(vertices); i++)
	{
		//座標コピー
		//vertMap[i] = pv[i];
		vertMap[i] = vertices[i];
	}

	//つながりを削除
	vertBuff->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;

	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResouceDesc{};
	cbResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResouceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResouceDesc.Height = 1;
	cbResouceDesc.DepthOrArraySize = 1;
	cbResouceDesc.MipLevels = 1;
	cbResouceDesc.SampleDesc.Count = 1;
	cbResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = spriteCommon->dxCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	//ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される 
	constMapTransform->color = MyMath::float4(1, 1, 1, 0.5f);	//普通の色

	constMapTransform->mat = Matrix();

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	UINT sizeIB = static_cast<UINT>(sizeof(Vertex) * _countof(indices));

	//インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	result = spriteCommon_->dxCommon_->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	//ScreenVertex* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (size_t i = 0; i < _countof(indices); i++)
	{
		//インデックスをコピー
		indexMap[i] = indices[i];
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

void Sprite::Update()
{
	constMapTransform->mat.Identity();

	//Initialize
	Matrix matScale, matRot, matTrans;
	this->matWorld.Identity();

	//scale
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;

	//rot
	matTrans.Identity();
	Matrix matRotX;
	matRotX.Identity();
	matRotX.m[1][1] = cos(rotation.x);
	matRotX.m[1][2] = sin(rotation.x);
	matRotX.m[2][1] = -sin(rotation.x);
	matRotX.m[2][2] = cos(rotation.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotation.y);
	matRotY.m[2][0] = sin(rotation.y);
	matRotY.m[0][2] = -sin(rotation.y);
	matRotY.m[2][2] = cos(rotation.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotation.z);
	matRotZ.m[0][1] = sin(rotation.z);
	matRotZ.m[1][0] = -sin(rotation.z);
	matRotZ.m[1][1] = cos(rotation.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//trans
	matTrans.Identity();
	matTrans.m[3][0] = position.x;
	matTrans.m[3][1] = position.y;
	matTrans.m[3][2] = position.z;

	//合体
	this->matWorld = matScale;
	this->matWorld *= matRot;
	this->matWorld *= matTrans;

	//////射影変換行列
	matProjection_ = MyMath::OrthoLH
	(Window::window_width,
		Window::window_height,
		0.0f,
		1.0f);

	//constMapTransform->color = color;

	constMapTransform->mat.m[0][0] = 2.0f / Window::window_width;
	constMapTransform->mat.m[1][1] = -2.0f / Window::window_height;

	constMapTransform->mat *= matWorld;
	constMapTransform->mat *= matProjection_;

	constMapTransform->mat.m[1][1] = -constMapTransform->mat.m[1][1];

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	constBuffTransform->Unmap(0, nullptr);
}

void Sprite::PreDraw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->SetPipelineState(spriteCommon_->pipelineState);
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootSignature(spriteCommon_->rootSignature);

	//プリミティブ形状の設定コマンド
	//三角形リスト
	//spriteCommon_->dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteCommon_->dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::Draw(size_t handle)
{
	//頂点バッファビューの設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	//デスクリプタ
	//spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1,);

	//画像を引数に入れたものに
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1, spriteCommon_->dxCommon_->GetTextureHandle(handle));

	//定数バッファビュー(CBV)の設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCmdList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::Draw()
{
	//頂点バッファビューの設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	//画像を引数に入れたものに
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1, spriteCommon_->dxCommon_->GetTextureHandle(tex));

	//定数バッファビュー(CBV)の設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCmdList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::LoadFile(const wchar_t* textureName)
{
	tex = spriteCommon_->dxCommon_->LoadTextureGraph(textureName);
}

void Sprite::TransferSpriteVertex(Vector2D size_)
{
	//size = size_;

	//// 左下、左上、右下、右上
	//enum { LB, LT, RB, RT };

	//float left = 0.0f * size.x;
	//float right = 1.0f * size.x;
	//float top = 0.0f * size.y;
	//float bottom = 1.0f * size.y;

	//pv[LB].pos = { left,	bottom,	0.0f }; // 左下
	//pv[LT].pos = { left,	top,	0.0f }; // 左上
	//pv[RB].pos = { right,	bottom,	0.0f }; // 右下
	//pv[RT].pos = { right,	top,	0.0f }; // 右上

	//float tex_left = 0.0f;
	//float tex_right = 1.0f;
	//float tex_top = 0.0f;
	//float tex_bottom = 1.0f;

	//pv[LB].uv = { tex_left,	tex_bottom }; // 左下
	//pv[LT].uv = { tex_left,	tex_top }; // 左上
	//pv[RB].uv = { tex_right,	tex_bottom }; // 右下
	//pv[RT].uv = { tex_right,	tex_top }; // 右上
}