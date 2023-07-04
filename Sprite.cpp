#include "Sprite.h"
#include <DirectXTex.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	//delete spriteCommon_;
}

void Sprite::Inilialize(SpriteCommon* spriteCommon)
{
	//spriteCommon_ = new SpriteCommon;
	spriteCommon_ = spriteCommon;

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

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
	result = spriteCommon->dxCommon_->GetDevice()->CreateCommittedResource(
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
	//Vector3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//全頂点に対して
	for (auto i = 0; i < _countof(vertices); i++)
	{
		//座標コピー
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
	result = spriteCommon->dxCommon_->GetDevice()->CreateCommittedResource(
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
	constMapTransform->color = XMFLOAT4(1, 1, 1, 0.5f);	//普通の色

	constMapTransform->mat = XMMatrixIdentity();

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	UINT sizeIB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(indices));

	//インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	result = spriteCommon_->dxCommon_->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
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

void Sprite::Inilialize()
{
}

void Sprite::Update(XMMATRIX& matView)
{
	XMMATRIX matScale, matRot, matTrans;

	//スケールなどの計算
	matScale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(this->rotation.z);
	matRot *= XMMatrixRotationX(this->rotation.x);
	matRot *= XMMatrixRotationY(this->rotation.y);

	matTrans = XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	this->matWorld = XMMatrixIdentity();
	this->matWorld *= matScale;
	this->matWorld *= matRot;
	this->matWorld *= matTrans;

	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / WindowApi::window_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / WindowApi::window_height;

	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

	XMMATRIX matProjection = XMMatrixIdentity();

	////射影変換行列
	//matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),
	//	(float)spriteCommon_->dxCommon_->GetWindow()->window_width / spriteCommon_->dxCommon_->GetWindow()->window_height,
	//	0.1f, 1000.0f);

	//↑のまとめ
	matProjection = XMMatrixOrthographicOffCenterLH
	(0,
		WindowApi::Get()->window_width,
		WindowApi::Get()->window_height,
		0,
		0.0f,
		1.0f);

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	constMapTransform->mat = this->matWorld * matProjection;

	constBuffTransform->Unmap(0, nullptr);
}

void Sprite::PreDraw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	spriteCommon_->dxCommon_->GetCommandList()->SetPipelineState(spriteCommon_->pipelineState);
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->rootSignature);

	//プリミティブ形状の設定コマンド
	//三角形リスト
	//spriteCommon_->dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	spriteCommon_->dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::Draw(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	//頂点バッファビューの設定コマンド
	spriteCommon_->dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//デスクリプタ
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1,);

	//画像を1に入れたものに
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, handle);

	//定数バッファビュー(CBV)の設定コマンド
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void Sprite::LoadResource()
{
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのコード
	result = LoadFromWICFile(
		L"Resources/reimu.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	ScratchImage mipChain{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//読み込んだディヒューズテクスチャをSRGBとして扱う
	metadata.format = MakeSRGB(metadata.format);

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc{};
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;
	textureResouceDesc.Height = (UINT)metadata.height;
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	srvDesc.Format = resDesc.Format;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//テクスチャヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	result = spriteCommon_->dxCommon_->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	//全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定を元にSRV用のデスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = spriteCommon_->dxCommon_->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//ハンドルの指す位置にシェーダーリソースビュー作成
	spriteCommon_->dxCommon_->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
}
