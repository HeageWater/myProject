#include "Model.h"
#include "ObjFile.h"

void Model::Initialize(Shader shader, const std::string& filename)
{
	D3D12_HEAP_PROPERTIES heapProp{};
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

	ObjFile objfile(filename, vertices_);
	vertexSize_ = (UINT)vertices_.size();

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertexSize_);

	VBInitialize(dx_->GetDev(), sizeVB, vertexSize_);

	mat_.Initialize();
}

void Model::Initialize(MyDirectX* dx, GPipeline* pipeline)
{
	dx_ = dx;
	pipeline_ = pipeline;
}

void Model::Initialize(MyDirectX* dx, Shader shader, const std::string& filename, GPipeline* pipeline)
{
	dx_ = dx;
	pipeline_ = pipeline;
	Initialize(shader, filename);
}

Model::Model()
{

}

Model::Model(MyDirectX* dx, Shader shader, const std::string& filename, GPipeline* pipeline)
{
	dx_ = dx;
	pipeline_ = pipeline;
	Initialize(shader, filename);
}

void Model::MatUpdate(Matrix matView, Matrix matProjection)
{
	mat_.Update();

	constMapTransform_->mat_ = mat_.matWorld_;
	constMapTransform_->mat_ *= matView;
	constMapTransform_->mat_ *= matProjection;
}

void Model::Update()
{
	mat_.Update();

	constMapTransform_->mat_ = mat_.matWorld_;
	constMapTransform_->mat_ *= View_;
	constMapTransform_->mat_ *= Prodaction_;
}

void Model::Draw(size_t handle)
{
	pipeline_->Setting(dx_->GetCmdList());
	pipeline_->Update(dx_->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx_->GetCmdList());
	//	テクスチャ
	dx_->GetCmdList()->SetGraphicsRootDescriptorTable(3, dx_->GetTextureHandle((int32_t)handle));
	dx_->GetCmdList()->SetGraphicsRootConstantBufferView(1, transform_->GetGPUVirtualAddress());

	dx_->GetCmdList()->DrawInstanced(vertexSize_, 1, 0, 0);
	//dx_->GetCmdList()->DrawIndexedInstanced(vertexSize_, 1, 0, 0, 0);
}

void Model::SetCamera(Matrix View, Matrix Projection)
{
	View_ = View;
	Prodaction_ = Projection;
}

void Model::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
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
