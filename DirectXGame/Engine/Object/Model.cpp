#include "Model.h"
#include "ObjFile.h"

void Model::Initialize(Shader shader, const char* filename)
{
	D3D12_HEAP_PROPERTIES heapProp_{};
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
	result_ = dx->GetDev()->CreateCommittedResource(
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

	ObjFile objfile(filename, vertices);
	vertexSize = (UINT)vertices.size();
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertexSize);

	VBInitialize(dx->GetDev(), sizeVB, vertexSize);

	mat.Initialize();
}

void Model::Initialize(MyDirectX* dx_, GPipeline* pipeline_)
{
	dx = dx_;
	pipeline = pipeline_;
}

void Model::Initialize(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_)
{
	dx = dx_;
	pipeline = pipeline_;
	Initialize(shader, filename);
}

Model::Model()
{
}

Model::Model(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_)
{
	dx = dx_;
	pipeline = pipeline_;
	Initialize(shader, filename);
}

void Model::MatUpdate(Matrix matView, Matrix matProjection)
{
	mat.Update();

	constMapTransform->mat = mat.matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Model::Draw(size_t handle)
{
	pipeline->Setting(dx->GetCmdList());
	pipeline->Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx->GetCmdList());
	//	テクスチャ
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle((int32_t)handle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	dx->GetCmdList()->DrawInstanced(vertexSize, 1, 0, 0);
}

void Model::SetVertices()
{
	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
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
