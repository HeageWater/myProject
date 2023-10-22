#include "Object2D.h"
#include "MyMath.h"

void Object2D::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexObj* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (size_t i = 0; i < vertexSize_; i++) {
		vertMap[i] = vertices_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}

void Object2D::SetMatScaling()
{
	matScale_.Identity();
	matScale_.m_[0][0] = scale_.x_;
	matScale_.m_[1][1] = scale_.y_;
	matScale_.m_[2][2] = scale_.z_;
}

void Object2D::SetMatRotation()
{
	matRot_.Identity();
	Matrix matRotX;
	matRotX.m_[1][1] = cos(rotAngle_.x_);
	matRotX.m_[1][2] = sin(rotAngle_.x_);
	matRotX.m_[2][1] = -sin(rotAngle_.x_);
	matRotX.m_[2][2] = cos(rotAngle_.x_);
	Matrix matRotY;
	matRotY.m_[0][0] = cos(rotAngle_.y_);
	matRotY.m_[2][0] = sin(rotAngle_.y_);
	matRotY.m_[0][2] = -sin(rotAngle_.y_);
	matRotY.m_[2][2] = cos(rotAngle_.y_);
	Matrix matRotZ;
	matRotZ.m_[0][0] = cos(rotAngle_.z_);
	matRotZ.m_[0][1] = sin(rotAngle_.z_);
	matRotZ.m_[1][0] = -sin(rotAngle_.z_);
	matRotZ.m_[1][1] = cos(rotAngle_.z_);

	matRot_ = matRotZ;
	matRot_ *= matRotX;
	matRot_ *= matRotY;
}

void Object2D::SetMatTransform()
{
	matTrans_.Identity();
	matTrans_.m_[3][0] = trans_.x_;
	matTrans_.m_[3][1] = trans_.y_;
	matTrans_.m_[3][2] = trans_.z_;
}

Object2D::Object2D(ID3D12Device* dev, Shader shader, UINT vertexNum, float rad)
{
#pragma region  ConstBuffer
	//D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	//	ヒープ設定
	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc_.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc_.Height = 1;
	cbResourceDesc_.DepthOrArraySize = 1;
	cbResourceDesc_.MipLevels = 1;
	cbResourceDesc_.SampleDesc.Count = 1;
	cbResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	生成
	result_ = dev->CreateCommittedResource(
		&cbHeapProp_,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform_));
	assert(SUCCEEDED(result_));

	//	定数バッファのマッピング
	result_ = transform_->Map(0, nullptr, (void**)&constMapTransform_);	//	マッピング
	assert(SUCCEEDED(result_));
#pragma endregion

	vertexSize_ = vertexNum;
	if (vertexNum >= 3)vertexSize_ += 1;

	//	インデックスサイズ

	indexSize_ = vertexNum * 3;
	indices_.resize(indexSize_);
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize_);

	int32_t num = vertexNum;

	for (int16_t i = 0; i < num; i++)
	{
		int32_t n = i * 3;
		indices_[n + 2] = (unsigned short)vertexSize_;
		indices_[n] = i;
		if (i == (int32_t)vertexNum - 1) {
			indices_[n + 1] = 0;
		}
		else {
			indices_[n + 1] = i + 1;
		}
	}

	// 頂点データサイズ
	vertices_.resize(vertexSize_);

	float radian = MyMath::PI * 2 / (float)vertexNum;

	for (size_t i = 0; i < vertexNum; i++)
	{
		vertices_[i].pos_.x_ = rad * sin(radian * i);
		vertices_[i].pos_.y_ = rad * cos(radian * i);
		vertices_[i].pos_.z_ = 0;
	}
	vertices_[vertexNum].pos_.x_ = 0;
	vertices_[vertexNum].pos_.y_ = 0;
	vertices_[vertexNum].pos_.z_ = 0;

	for (size_t i = 0; i < vertexNum; i++)
	{
		if (i % 3 == 0) {
			vertices_[i].color_.x_ = 1.0f;
			vertices_[i].color_.y_ = 0.0f;
			vertices_[i].color_.z_ = 0.0f;
		}
		else if (i % 3 == 1) {
			vertices_[i].color_.x_ = 0.0f;
			vertices_[i].color_.y_ = 1.0f;
			vertices_[i].color_.z_ = 0.0f;
		}
		else {
			vertices_[i].color_.x_ = 0.0f;
			vertices_[i].color_.y_ = 0.0f;
			vertices_[i].color_.z_ = 1.0f;
		}
		vertices_[i].color_.w_ = 1.0f;
	}
	vertices_[vertexNum].color_.x_ = 1.0f;
	vertices_[vertexNum].color_.y_ = 1.0f;
	vertices_[vertexNum].color_.z_ = 1.0f;
	vertices_[vertexNum].color_.w_ = 1.0f;

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertexSize_);
	VBInitialize(dev, sizeVB, sizeIB, &indices_.front(), indexSize_);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	色
	};

	pipeline_.Init(dev, shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	pipeline_.SetBlend(dev, GPipeline::ALPHA_BLEND);
#pragma region  WorldMatrix初期値
	scale_ = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle_ = Vector3D(0.0f, 0.0f, 0.0f);
	trans_ = Vector3D(0.0f, 0.0f, 0.0f);
#pragma endregion
}

void Object2D::MatUpdate(Matrix matView, Matrix matProjection)
{
#pragma region WorldMatrix
	matWorld_.Identity();

	//	スケーリング
	SetMatScaling();
	matWorld_ *= matScale_;

	//	回転
	SetMatRotation();
	matWorld_ *= matRot_;

	//	平行移動
	SetMatTransform();
	matWorld_ *= matTrans_;
#pragma endregion

	constMapTransform_->mat_ = matWorld_;
	constMapTransform_->mat_ *= matView;
	constMapTransform_->mat_ *= matProjection;
}

void Object2D::Draw(ID3D12GraphicsCommandList* cmdList)
{
	pipeline_.Setting(cmdList);
	pipeline_.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(cmdList);
	cmdList->SetGraphicsRootConstantBufferView(2, transform_->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0);
}
