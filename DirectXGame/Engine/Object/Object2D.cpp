#include "Object2D.h"
#include "MyMath.h"

void Object2D::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	VertexObj* vertMap = nullptr;
	result_ = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (size_t i = 0; i < vertexSize; i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
}

void Object2D::SetMatScaling()
{
	matScale.Identity();
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
}

void Object2D::SetMatRotation()
{
	matRot.Identity();
	Matrix matRotX;
	matRotX.m[1][1] = cos(rotAngle.x);
	matRotX.m[1][2] = sin(rotAngle.x);
	matRotX.m[2][1] = -sin(rotAngle.x);
	matRotX.m[2][2] = cos(rotAngle.x);
	Matrix matRotY;
	matRotY.m[0][0] = cos(rotAngle.y);
	matRotY.m[2][0] = sin(rotAngle.y);
	matRotY.m[0][2] = -sin(rotAngle.y);
	matRotY.m[2][2] = cos(rotAngle.y);
	Matrix matRotZ;
	matRotZ.m[0][0] = cos(rotAngle.z);
	matRotZ.m[0][1] = sin(rotAngle.z);
	matRotZ.m[1][0] = -sin(rotAngle.z);
	matRotZ.m[1][1] = cos(rotAngle.z);

	matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;
}

void Object2D::SetMatTransform()
{
	matTrans.Identity();
	matTrans.m[3][0] = trans.x;
	matTrans.m[3][1] = trans.y;
	matTrans.m[3][2] = trans.z;
}

Object2D::Object2D(ID3D12Device* dev, Shader shader, UINT vertexNum, float rad)
{
#pragma region  ConstBuffer
	//D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	//	ヒープ設定
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	生成
	result_ = dev->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(result_));

	//	定数バッファのマッピング
	result_ = transform->Map(0, nullptr, (void**)&constMapTransform);	//	マッピング
	assert(SUCCEEDED(result_));
#pragma endregion

	vertexSize = vertexNum;
	if (vertexNum >= 3)vertexSize += 1;

	//	インデックスサイズ

	indexSize = vertexNum * 3;
	indices.resize(indexSize);
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);

	int32_t num = vertexNum;

	for (int16_t i = 0; i < num; i++)
	{
		int32_t n = i * 3;
		indices[n + 2] = (unsigned short)vertexSize;
		indices[n] = i;
		if (i == (int32_t)vertexNum - 1) {
			indices[n + 1] = 0;
		}
		else {
			indices[n + 1] = i + 1;
		}
	}

	// 頂点データサイズ
	vertices.resize(vertexSize);

	float radian = MyMath::PI * 2 / (float)vertexNum;

	for (size_t i = 0; i < vertexNum; i++)
	{
		vertices[i].pos.x = rad * sin(radian * i);
		vertices[i].pos.y = rad * cos(radian * i);
		vertices[i].pos.z = 0;
	}
	vertices[vertexNum].pos.x = 0;
	vertices[vertexNum].pos.y = 0;
	vertices[vertexNum].pos.z = 0;

	for (size_t i = 0; i < vertexNum; i++)
	{
		if (i % 3 == 0) {
			vertices[i].color.x = 1.0f;
			vertices[i].color.y = 0.0f;
			vertices[i].color.z = 0.0f;
		}
		else if (i % 3 == 1) {
			vertices[i].color.x = 0.0f;
			vertices[i].color.y = 1.0f;
			vertices[i].color.z = 0.0f;
		}
		else {
			vertices[i].color.x = 0.0f;
			vertices[i].color.y = 0.0f;
			vertices[i].color.z = 1.0f;
		}
		vertices[i].color.w = 1.0f;
	}
	vertices[vertexNum].color.x = 1.0f;
	vertices[vertexNum].color.y = 1.0f;
	vertices[vertexNum].color.z = 1.0f;
	vertices[vertexNum].color.w = 1.0f;

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);
	VBInitialize(dev, sizeVB, sizeIB, &indices.front(), indexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	色
	};

	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE);
	pipeline.SetBlend(dev, GPipeline::ALPHA_BLEND);
#pragma region  WorldMatrix初期値
	scale = Vector3D(1.0f, 1.0f, 1.0f);
	rotAngle = Vector3D(0.0f, 0.0f, 0.0f);
	trans = Vector3D(0.0f, 0.0f, 0.0f);
#pragma endregion
}

void Object2D::MatUpdate(Matrix matView, Matrix matProjection)
{
#pragma region WorldMatrix
	matWorld.Identity();

	//	スケーリング
	SetMatScaling();
	matWorld *= matScale;

	//	回転
	SetMatRotation();
	matWorld *= matRot;

	//	平行移動
	SetMatTransform();
	matWorld *= matTrans;
#pragma endregion

	constMapTransform->mat = matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Object2D::Draw(ID3D12GraphicsCommandList* cmdList)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(cmdList);
	cmdList->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());
	cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}
