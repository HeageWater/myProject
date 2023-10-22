#include "VertBuff.h"
#include <wrl.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

VertBuff::VertBuff()
{
}

void VertBuff::SetLighting()
{
	//for (size_t i = 0; i < indicesSize / 3; i++)
	//{
	//	unsigned short index0 = indices[i * 3];
	//	unsigned short index1 = indices[i * 3 + 1];
	//	unsigned short index2 = indices[i * 3 + 2];

	//	Vector4D p0(vertices[index0].pos, 0.0f);
	//	Vector4D p1(vertices[index1].pos, 0.0f);
	//	Vector4D p2(vertices[index2].pos, 0.0f);

	//	Vector4D v1 = p1 - p0;
	//	Vector4D v2 = p2 - p0;
	//	Vector4D normal = v1 - v2;

	//	normal.normalize();
	//	Vector3D normal3D(normal.x, normal.y, normal.z);
	//	vertices[index0].normal = normal3D;
	//	vertices[index1].normal = normal3D;
	//	vertices[index2].normal = normal3D;
	//}
}

void VertBuff::VertBuffUpdate(ID3D12GraphicsCommandList* cmdList)
{
	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	//	インデックスバッファビュー設定コマンド
	if (ibExist_) {
		cmdList->IASetIndexBuffer(&ibView_);
	}
}

void VertBuff::SetVertices()
{
}

//void VertBuff::VBInitialize(ID3D12Device* dev, UINT sizeVB, UINT vertSize, UINT sizeIB, uint16_t* indices, UINT indicesSize)
void VertBuff::VBInitialize(ID3D12Device* dev, UINT sizeVB, UINT sizeIB, uint16_t* indices, UINT indicesSize)
{
	SetLighting();

	//	ヒープの設定
	heapProp_.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用(CPUからアクセスできる)

#pragma region VB
	// リソース設定
	SetResDesc(sizeVB);

	//	GPU側にメモリ確保
	result_ = dev->CreateCommittedResource(
		&heapProp_,							// ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_,							// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result_));

	// 頂点バッファビューの作成(GPUで利用するため)
	// GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;

	SetVertices();
#pragma endregion

#pragma region IB
	if (indices != nullptr) {
		ibExist_ = true;

		SetResDesc(sizeIB);
		result_ = dev->CreateCommittedResource(
			&heapProp_, // ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc_, // リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff_));
		assert(SUCCEEDED(result_));
		//	インデックスバッファマッピング
		uint16_t* indexMap = nullptr;
		result_ = indexBuff_->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result_));
		// 全頂点に対して
		for (size_t i = 0; i < indicesSize; i++) {
			indexMap[i] = indices[i]; // 座標をコピー
		}
		// 繋がりを解除
		indexBuff_->Unmap(0, nullptr);
		//	インデックスバッファビュー作成
		ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
		ibView_.Format = DXGI_FORMAT_R16_UINT;
		ibView_.SizeInBytes = sizeIB;
	}
#pragma endregion
}

void VertBuff::SetResDesc(UINT size)
{
	resDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc_.Width = size; // 頂点データ全体のサイズ
	resDesc_.Height = 1;
	resDesc_.DepthOrArraySize = 1;
	resDesc_.MipLevels = 1;
	resDesc_.SampleDesc.Count = 1;		//	アンチエイリアシング用のパラメータ
	resDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void VertBuff::BuffTransferGPU(ID3D12Resource* buff, ID3D12Device* dev)
{
#pragma region バッファの生成
	result_ = dev->CreateCommittedResource(
		&heapProp_, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc_, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result_));
#pragma endregion
}
