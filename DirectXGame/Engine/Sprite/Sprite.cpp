#include "Sprite.h"
#include <DirectXTex.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	//delete spriteCommon_;
}

void Sprite::Inilialize(SpriteCommon* spriteCommon, Matrix* matProjection)
{
	//spriteCommon_ = new SpriteCommon;
	this->spriteCommon_ = spriteCommon;
	this->matProjection__ = *matProjection;

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//UINT sizeVB = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));
	//VBInitialize(dx->GetDev(), sizePV, vertexSize);
	//頂点バッファの設定
	//ヒープの設定
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPUへの転送用
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	//D3D12_RESOURCE_DESC resDesc{};
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = sizeVB;
	//resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;;
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result_ = spriteCommon->dxCommon_->GetDev()->CreateCommittedResource(
		//ヒープ設定
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		//リソース設定
		&resDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result_));

	//gpu状のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	//ScreenVertex* vertMap = nullptr;
	result_ = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));

	//全頂点に対して
	//for (size_t i = 0; i < vertexSize; i++)
	for (size_t i = 0; i < _countof(vertices_); i++)
	{
		//座標コピー
		//vertMap[i] = pv[i];
		vertMap[i] = vertices_[i];
	}

	//つながりを削除
	vertBuff->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView_.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;

	//頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);

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
	result_ = spriteCommon->dxCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform_));
	assert(SUCCEEDED(result_));

	//定数バッファのマッピング
	//ConstBufferDataMaterial* constMapMaterial = nullptr;
	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	//値を書き込むと自動的に転送される 
	constMapTransform_->color_ = MyMath::float4(1, 1, 1, 0.5f);	//普通の色

	constMapTransform_->mat_ = Matrix();

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	UINT sizeIB = static_cast<UINT>(sizeof(Vertex) * _countof(indices_));

	//インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	result_ = spriteCommon_->dxCommon_->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	//ScreenVertex* indexMap = nullptr;
	result_ = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (size_t i = 0; i < _countof(indices_); i++)
	{
		//インデックスをコピー
		indexMap[i] = indices_[i];
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
	constMapTransform_->mat_.Identity();

	//Initialize
	Matrix matScale, matRot, matTrans;
	this->matWorld_.Identity();

	//scale
	matScale.Identity();
	matScale.m_[0][0] = scale_.x_;
	matScale.m_[1][1] = scale_.y_;
	matScale.m_[2][2] = scale_.z_;

	//rot
	matTrans.Identity();
	Matrix matRotX;
	matRotX.Identity();
	matRotX.m_[1][1] = cos(rotation_.x_);
	matRotX.m_[1][2] = sin(rotation_.x_);
	matRotX.m_[2][1] = -sin(rotation_.x_);
	matRotX.m_[2][2] = cos(rotation_.x_);
	Matrix matRotY;
	matRotY.m_[0][0] = cos(rotation_.y_);
	matRotY.m_[2][0] = sin(rotation_.y_);
	matRotY.m_[0][2] = -sin(rotation_.y_);
	matRotY.m_[2][2] = cos(rotation_.y_);
	Matrix matRotZ;
	matRotZ.m_[0][0] = cos(rotation_.z_);
	matRotZ.m_[0][1] = sin(rotation_.z_);
	matRotZ.m_[1][0] = -sin(rotation_.z_);
	matRotZ.m_[1][1] = cos(rotation_.z_);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//trans
	matTrans.Identity();
	matTrans.m_[3][0] = position_.x_;
	matTrans.m_[3][1] = position_.y_;
	matTrans.m_[3][2] = position_.z_;

	//合体
	this->matWorld_ = matScale;
	this->matWorld_ *= matRot;
	this->matWorld_ *= matTrans;

	//////射影変換行列
	matProjection__ = MyMath::OrthoLH
	(Window::window_width_,
		Window::window_height_,
		0.0f,
		1.0f);

	constMapTransform_->mat_.m_[0][0] = 2.0f / Window::window_width_;
	constMapTransform_->mat_.m_[1][1] = -2.0f / Window::window_height_;

	constMapTransform_->mat_ *= matWorld_;
	constMapTransform_->mat_ *= matProjection__;

	constMapTransform_->mat_.m_[1][1] = -constMapTransform_->mat_.m_[1][1];

	result_ = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);
	assert(SUCCEEDED(result_));

	constBuffTransform_->Unmap(0, nullptr);
}

void Sprite::PreDraw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->SetPipelineState(spriteCommon_->pipelineState_);
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootSignature(spriteCommon_->rootSignature_);

	//プリミティブ形状の設定コマンド
	//三角形リスト
	//spriteCommon_->dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteCommon_->dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::Draw(size_t handle)
{
	//
	PreDraw();

	//頂点バッファビューの設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView_);\

	//画像を引数に入れたものに
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1, spriteCommon_->dxCommon_->GetTextureHandle(handle));

	//定数バッファビュー(CBV)の設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());

	//描画コマンド
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCmdList()->DrawIndexedInstanced(_countof(indices_), 1, 0, 0, 0);
}

void Sprite::Draw()
{
	//頂点バッファビューの設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView_);

	//画像を引数に入れたものに
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(1, spriteCommon_->dxCommon_->GetTextureHandle(tex_));

	//定数バッファビュー(CBV)の設定コマンド
	spriteCommon_->dxCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, constBuffTransform_->GetGPUVirtualAddress());

	//描画コマンド
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCmdList()->DrawIndexedInstanced(_countof(indices_), 1, 0, 0, 0);
}

void Sprite::LoadFile(const wchar_t* textureName)
{
	tex_ = spriteCommon_->dxCommon_->LoadTextureGraph(textureName);
}

void Sprite::SetColor(Vector4D color)
{
	constMapTransform_->color_ = { color.x_, color.y_ ,color.z_ ,color.w_ };
}