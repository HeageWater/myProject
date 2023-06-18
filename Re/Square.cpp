#include "Square.h"

UISquare::UISquare(ID3D12Device* dev, Shader shader)
{
	//HRESULT result;

	pv[0] = { {-1.0f,-1.0f,0.1f},{0,1} };
	pv[1] = { {-1.0f, 1.0f,0.1f},{0,0} };
	pv[2] = { { 1.0f,-1.0f,0.1f},{1,1} };
	pv[3] = { { 1.0f, 1.0f,0.1f},{1,0} };
	vertexSize = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	VBInitialize(dev, sizePV, vertexSize);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};
	pipeline.Init(dev, shader, inputLayout, _countof(inputLayout));
	pipeline.SetBlend(dev, GPipeline::ALPHA_BLEND);
}

void UISquare::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(pv[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	ScreenVertex* vertMap = nullptr;
	HRESULT newresult = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(newresult));
	// 全頂点に対して
	for (size_t i = 0; i < (signed)vertexSize; i++) {
		vertMap[i] = pv[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}

void UISquare::Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	pipeline.Setting(cmdList);
	pipeline.Update(cmdList, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	VertBuffUpdate(cmdList);
	//	テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, handle);
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void Square::Initialize(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, size_t blendMord)
{
	dx = dx_;
	pipeline = pipeline_;

	HRESULT newresult;

#pragma region  ConstBuffer
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
	newresult = dx->GetDev()->CreateCommittedResource(
		&cbHeapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform));
	assert(SUCCEEDED(newresult));

	//	定数バッファのマッピング
	newresult = transform->Map(0, nullptr, (void**)&constMapTransform);	//	マッピング
	assert(SUCCEEDED(newresult));

	//	ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU転送用

	//	リソース設定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF;
	//	生成
	newresult = dx->GetDev()->CreateCommittedResource(
		&heapProp,	//	ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,	//	リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material));
	assert(SUCCEEDED(newresult));

	//	定数バッファのマッピング
	newresult = material->Map(0, nullptr, (void**)&mapMaterial);	//	マッピング
	assert(SUCCEEDED(newresult));
#pragma endregion
	pv[0] = { {-1.0f,-1.0f,0.0f},{},{0,1} };
	pv[1] = { {-1.0f, 1.0f,0.0f},{},{0,0} };
	pv[2] = { { 1.0f,-1.0f,0.0f},{},{1,1} };
	pv[3] = { { 1.0f, 1.0f,0.0f},{},{1,0} };
	vertexSize = 4;
	UINT sizePV = static_cast<UINT>(sizeof(pv[0]) * vertexSize);
	indexSize = 6;
	//	インデックスデータ
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indexSize);
	VBInitialize(dx->GetDev(), sizePV, vertexSize, sizeIB, indices, indexSize);

	if (blendMord != GPipeline::NONE_BLEND) {
		pipeline->SetBlend(dx->GetDev(), blendMord);
	}
#pragma region  WorldMatrix初期値
	obj.Initialize();
#pragma endregion
}

Square::Square(MyDirectX* dx_, GPipeline* pipeline_, Shader shader, size_t blendMord)
{
	Initialize(dx_, pipeline_, shader, blendMord);
}

Square::Square()
{
}

void Square::MatUpdate(Matrix matView, Matrix matProjection, float a, Matrix billboard)
{
	obj.trans.x_ -= a;

	obj.Update(billboard);

	constMapTransform->mat = obj.matWorld;
	constMapTransform->mat *= matView;
	constMapTransform->mat *= matProjection;
}

void Square::Draw(size_t handle)
{
	pipeline->Setting(dx->GetCmdList());
	pipeline->Update(dx->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx->GetCmdList());
	//	テクスチャ
	dx->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx->GetTextureHandle(handle));
	dx->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform->GetGPUVirtualAddress());

	dx->GetCmdList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
}

void Square::SetVertices()
{
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(pv[0]);

	//	GPUメモリの値書き換えよう
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	HRESULT newresult = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(newresult));
	// 全頂点に対して
	for (size_t i = 0; i < (signed)vertexSize; i++) {
		vertMap[i] = pv[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
}
