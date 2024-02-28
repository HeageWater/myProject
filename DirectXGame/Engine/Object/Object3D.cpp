#include "Object3D.h"
#include <cmath>
#include "MyMath.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")

void Object3D::Initialize(Shader shader)
{
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

	vertexSize_ = 24;
	vertices_.resize(vertexSize_);
	// 頂点データ
	vertices_ = {
		//	前
		{{ -1.0f,-1.0f,-1.0f },{},{0.0f, 1.0f}}, // 左下
		{{ -1.0f, 1.0f,-1.0f },{},{0.0f, 0.0f}}, // 左上
		{{  1.0f,-1.0f,-1.0f },{},{1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f,-1.0f },{},{1.0f, 0.0f}}, // 右上
		//	後
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{ -1.0f, 1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f,-1.0f, 1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	左
		{{ -1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 右下
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左下
		{{ -1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 左上
		{{ -1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	右
		{{  1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{  1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	下
		{{ -1.0f,-1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{ -1.0f,-1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f,-1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f,-1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
		//	上
		{{ -1.0f, 1.0f,-1.0f },{}, {0.0f, 1.0f}}, // 左下
		{{ -1.0f, 1.0f, 1.0f },{}, {0.0f, 0.0f}}, // 左上
		{{  1.0f, 1.0f,-1.0f },{}, {1.0f, 1.0f}}, // 右下
		{{  1.0f, 1.0f, 1.0f },{}, {1.0f, 0.0f}}, // 右上
	};
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertexSize_);
	indexSize_ = 36;
	//	インデックスデータ
	indices_ =
	{
		0,1,2,
		2,1,3,
		6,5,4,
		7,5,6,
		8,9,10,
		10,9,11,
		14,13,12,
		15,13,14,
		18,17,16,
		19,17,18,
		20,21,22,
		22,21,23,
	};
	//	全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize_);
	VBInitialize(dx_->GetDev(), sizeVB, sizeIB, &indices_.front(), indexSize_);
	//VBInitialize(dx->GetDev(), sizeVB, vertexSize, sizeIB, &indices.front(), indexSize);

	mat_.Initialize();
}

void Object3D::Initialize(MyDirectX* dx, GPipeline* pipeline, Shader shader)
{
	dx_ = dx;
	pipeline_ = pipeline;
	Initialize(shader);
}

Object3D::Object3D()
{
}

Object3D::Object3D(MyDirectX* dx_, GPipeline* pipeline_, Shader shader)
{
	dx_ = dx_;
	pipeline_ = pipeline_;
	Initialize(shader);
}


void Object3D::MatUpdate(Matrix matView, Matrix matProjection)
{
	mat_.Update();

	constMapTransform_->mat_ = mat_.matWorld_;
	constMapTransform_->mat_ *= matView;
	constMapTransform_->mat_ *= matProjection;
}

void Object3D::SetVertices()
{
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

void Object3D::Draw(size_t handle)
{
	//パイプラインセット
	pipeline_->Setting(dx_->GetCmdList());
	//パイプラインアップデート
	pipeline_->Update(dx_->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//バッファアップデート
	VertBuffUpdate(dx_->GetCmdList());

	// SRVヒープの先頭にあるSRVをルートパラメータ3番に設定
	dx_->GetCmdList()->SetGraphicsRootDescriptorTable(3, dx_->GetTextureHandle((int32_t)handle));
	// SRVヒープの設定コマンド
	dx_->GetCmdList()->SetGraphicsRootConstantBufferView(1, transform_->GetGPUVirtualAddress());

	// 描画コマンド
	dx_->GetCmdList()->DrawIndexedInstanced(indexSize_, 1, 0, 0, 0);
}
