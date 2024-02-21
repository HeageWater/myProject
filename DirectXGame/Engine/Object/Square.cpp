#include "Square.h"

UISquare::UISquare(ID3D12Device* dev, Shader shader)
{
	pv_[0] = { {-1.0f,-1.0f,0.1f},{0,1} };
	pv_[1] = { {-1.0f, 1.0f,0.1f},{0,0} };
	pv_[2] = { { 1.0f,-1.0f,0.1f},{1,1} };
	pv_[3] = { { 1.0f, 1.0f,0.1f},{1,0} };
	vertexSize_ = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv_[0]) * vertexSize_);
	VBInitialize(dev, sizePV, vertexSize_);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};
	pipeline_.Init(dev, shader, inputLayout, _countof(inputLayout));
	pipeline_.SetBlend(dev,GPipeline::ALPHA_BLEND);
}

void UISquare::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(pv_[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (size_t i = 0; i < vertexSize_; i++) {
		vertMap[i] = pv_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}

void UISquare::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	pipeline_.Setting(cmdList);
	pipeline_.Update(cmdList, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	VertBuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, handle);
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Square::Initialize(GPipeline* pipeline, Shader shader, size_t blendMord)
{
	dx_ = MyDirectX::GetInstance();
	pipeline_ = pipeline;

#pragma region  ConstBuffer
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
	result_ = dx_->GetDev()->CreateCommittedResource(
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

	//	ヒープ設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	resourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc_.Height = 1;
	resourceDesc_.DepthOrArraySize = 1;
	resourceDesc_.MipLevels = 1;
	resourceDesc_.SampleDesc.Count = 1;
	resourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc_.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	result_ = dx_->GetDev()->CreateCommittedResource(
		&heapProp_,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc_,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material_));
	assert(SUCCEEDED(result_));

	//	定数バッファのマッピング
	result_ = material_->Map(0, nullptr, (void**)&mapMaterial_);	//	マッピング
	assert(SUCCEEDED(result_));
#pragma endregion
	pv_[0] = { {-1.0f,-1.0f,0.0f},{},{0,1} };
	pv_[1] = { {-1.0f, 1.0f,0.0f},{},{0,0} };
	pv_[2] = { { 1.0f,-1.0f,0.0f},{},{1,1} };
	pv_[3] = { { 1.0f, 1.0f,0.0f},{},{1,0} };
	vertexSize_ = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv_[0]) * vertexSize_);
	indexSize_ = 6;
	//	インデックスデータ
	indices_[0] = 0;
	indices_[1] = 1;
	indices_[2] = 2;
	indices_[3] = 2;
	indices_[4] = 1;
	indices_[5] = 3;
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize_);
	VBInitialize(dx_->GetDev(), sizePV, sizeIB, indices_, indexSize_);
	//VBInitialize(dx->GetDev(), sizePV, vertexSize, sizeIB, indices, indexSize);

	if (blendMord != GPipeline::NONE_BLEND) {
		pipeline_->SetBlend(dx_->GetDev(), (int32_t)blendMord);
	}
#pragma region  WorldMatrix初期値
	obj_.Initialize();
#pragma endregion
}

Square::Square(GPipeline* pipeline_, Shader shader, size_t blendMord)
{
	Initialize(pipeline_, shader, blendMord);
}

Square::Square()
{
}

void Square::MatUpdate(Matrix matView, Matrix matProjection, Matrix billboard)
{
	obj_.Update(billboard);

	constMapTransform_->mat_ = obj_.matWorld_;
	constMapTransform_->mat_ *= matView;
	constMapTransform_->mat_ *= matProjection;
}

void Square::MatUpdate(Matrix matView, Matrix matProjection, float a, Matrix billboard)
{
	obj_.trans_.x_ -= a;

	obj_.Update(billboard);

	constMapTransform_->mat_ = obj_.matWorld_;
	constMapTransform_->mat_ *= matView;
	constMapTransform_->mat_ *= matProjection;
}

void Square::Draw(size_t handle)
{
	pipeline_->Setting(dx_->GetCmdList());
	pipeline_->Update(dx_->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx_->GetCmdList());
	//	テクスチャ
	dx_->GetCmdList()->SetGraphicsRootDescriptorTable(3, dx_->GetTextureHandle((int32_t)handle));
	dx_->GetCmdList()->SetGraphicsRootConstantBufferView(1, transform_->GetGPUVirtualAddress());
	
	dx_->GetCmdList()->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0);
}

void Square::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(pv_[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result_ = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result_));
	// 全頂点に対して
	for (size_t i = 0; i < vertexSize_; i++) {
		vertMap[i] = pv_[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff_->Unmap(0, nullptr);
}
