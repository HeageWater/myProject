//#include "SpriteCommon.h"
//
//SpriteCommon::SpriteCommon()
//{
//}
//
//SpriteCommon::~SpriteCommon()
//{
//}
//
//void SpriteCommon::Inilialize(DirectXCommon* dxCommon)
//{
//	dxCommon = new DirectXCommon();
//	dxCommon->Initialize();
//
//	assert(dxCommon);
//
//	this->dxCommon = *dxCommon;
//
//	//頂点データ構造体
//	struct Vertex
//	{
//		XMFLOAT3 pos;
//		XMFLOAT3 noemal;
//		XMFLOAT2 uv;
//	};
//
//	//頂点データ
//	Vertex vertices[] =
//	{
//		//	x,		y,		z,		u,	 v
//		//前
//		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
//		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
//		{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//右下
//		{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//右上 
//
//		//後
//		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},//左下
//		{{ -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},//左上
//		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
//		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上
//
//		//左
//		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
//		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
//		{{ -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
//		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上
//
//		//右
//		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
//		{{  5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
//		{{  5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
//		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上
//
//		//下
//		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
//		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,0.0f}},//左上
//		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
//		{{ -5.0f, -5.0f,  5.0f},{},{1.0f,0.0f}},//右上
//
//		//上
//		{{  5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},//左下
//		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
//		{{  5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},//右下
//		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上
//	};
//
//	//インデックスデータ
//	unsigned short indices[] =
//	{
//		//前
//		0,1,2,
//		2,1,3,
//		//後
//		6,5,4,
//		7,5,6,
//		//左
//		8,9,10,
//		11,10,9,
//		//右
//		14,13,12,
//		13,14,15,
//		//下
//		18,17,16,
//		17,18,19,
//		//上
//		20,21,22,
//		23,22,21,
//	};
//
//	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
//
//	//頂点バッファの設定
//	//ヒープの設定
//	D3D12_HEAP_PROPERTIES heapProp{};
//
//	//GPUへの転送用
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//
//	//リソース設定
//	D3D12_RESOURCE_DESC resDesc{};
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB;
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//頂点バッファの生成
//	ID3D12Resource* vertBuff = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		//ヒープ設定
//		&heapProp,
//		D3D12_HEAP_FLAG_NONE,
//
//		//リソース設定
//		&resDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//gpu状のバッファに対応した仮想メモリ(メインメモリ上)を取得
//	Vertex* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//
//	//全頂点に対して
//	for (int i = 0; i < _countof(vertices); i++)
//	{
//		//座標コピー
//		vertMap[i] = vertices[i];
//	}
//
//	//つながりを削除
//	vertBuff->Unmap(0, nullptr);
//
//	//頂点バッファビューの作成
//	//D3D12_VERTEX_BUFFER_VIEW vbView{};
//
//	//GPU仮想アドレス
//	vbView->BufferLocation = vertBuff->GetGPUVirtualAddress();
//
//	//頂点バッファのサイズ
//	vbView->SizeInBytes = sizeVB;
//
//	//頂点1つ分のデータサイズ
//	//vbView.StrideInBytes = sizeof(XMFLOAT3);
//	vbView->StrideInBytes = sizeof(vertices[0]);
//
//	//頂点シェーダオブジェクト
//	ID3DBlob* vsBlob = nullptr;
//
//	//ピクセルシェーダオブジェクト
//	ID3DBlob* psBlob = nullptr;
//
//	//エラーオブジェクト
//	ID3DBlob* errorBlob = nullptr;
//
//	//頂点シェーダの読み込みとコンパイル
//	result = D3DCompileFromFile(
//		L"BasicVS.hlsl",//シェーダファイル名
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードを可能にする
//		"main", "vs_5_0",//エントリーポインタ名、シェーダモデル指定
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
//		0,
//		&vsBlob, &errorBlob);
//
//	//エラーなら
//	if (FAILED(result)) {
//		//errorBlobからエラー内容をstring型にコピー
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		//エラー内容を出力ウィンドウに表示
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//	//ピクセルシェーダの読み込みとコンパイル
//	result = D3DCompileFromFile(
//		L"BasicPS.hlsl",//シェーダファイル名
//		nullptr,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードを可能にする
//		"main", "ps_5_0",//エントリーポインタ名、シェーダモデル指定
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
//		0,
//		&psBlob, &errorBlob);
//
//	//エラーなら
//	if (FAILED(result)) {
//		//errorBlobからエラー内容をstring型にコピー
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		//エラー内容を出力ウィンドウに表示
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//
//	//頂点レイアウト
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
//	{
//		{
//			"POSITION",
//			0,
//			DXGI_FORMAT_R32G32B32_FLOAT,
//			0,
//			D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//			0
//		},
//
//		{
//			"NORMAL",
//			0,
//			DXGI_FORMAT_R32G32B32_FLOAT,
//			0,
//			D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//			0
//		},
//
//		{
//			"TEXCOORD",
//			0,
//			DXGI_FORMAT_R32G32_FLOAT,
//			0,
//			D3D12_APPEND_ALIGNED_ELEMENT,
//			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
//			0
//		},
//	};
//
//	//グラフィックスパイプライン設定
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};
//
//	//シェーダーの設定
//	gpipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
//	gpipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
//	gpipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
//	gpipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
//
//	//サンプルマスクの設定
//	gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定
//
//	//ラスタライザの設定
//
//	//カリングしない
//	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
//	//ポリゴン内塗りつぶし
//	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
//	//深度クリッピングを有効に
//	gpipelineDesc.RasterizerState.DepthClipEnable = true;
//
//	//ブレンドステート
//	//gpipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
//	//	= D3D12_COLOR_WRITE_ENABLE_ALL;
//	//レンダーターゲットのブレンド設定
//	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipelineDesc.BlendState.RenderTarget[0];
//	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//
//	//ブレンドを無効
//	blenddesc.BlendEnable = false;
//
//	//デプスステンシルステートの作成
//	gpipelineDesc.DepthStencilState.DepthEnable = true;
//	gpipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
//	gpipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
//	gpipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
//
//	//RBGA全てのチャンネルを描画
//	//ポリゴン内塗りつぶし
//	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
//
//	//ワイヤーフレーム
//	//gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
//
//	//頂点レイアウトの設定
//	gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
//	gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);
//
//	//図形の形状設定
//	gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	//その他の設定
//	//描画対象は一つ
//	gpipelineDesc.NumRenderTargets = 1;
//	//0~255指定のRGBA
//	gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	//1ピクセルのつき1回サンプリング
//	gpipelineDesc.SampleDesc.Count = 1;
//
//	//ヒープ設定
//	D3D12_HEAP_PROPERTIES cbHeapProp{};
//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//リソース設定
//	D3D12_RESOURCE_DESC cbResouceDesc{};
//	cbResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	cbResouceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
//	cbResouceDesc.Height = 1;
//	cbResouceDesc.DepthOrArraySize = 1;
//	cbResouceDesc.MipLevels = 1;
//	cbResouceDesc.SampleDesc.Count = 1;
//	cbResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	ID3D12Resource* constBuffMaterial = nullptr;
//	//定数バッファの生成
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&cbHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&cbResouceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuffMaterial));
//	assert(SUCCEEDED(result));
//
//	//定数バッファのマッピング
//	ConstBufferDataMaterial* constMapMaterial = nullptr;
//	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
//	assert(SUCCEEDED(result));
//
//	//値を書き込むと自動的に転送される 
//	constMapMaterial->color = XMFLOAT4(1, 1, 1, 0.5f);	//普通の色
//
//	//デスクリプタレンジの設定
//	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
//	descriptorRange.NumDescriptors = 1;
//	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
//	descriptorRange.BaseShaderRegister = 0;
//	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//	//ルートパラメタの設定
//	D3D12_ROOT_PARAMETER rootParams[3] = {};
//
//	//定数バッファ0番
//	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
//	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
//	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
//	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
//	//テクスチャレジスタ0番
//	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//定数バッファビュー
//	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//定数バッファ番号
//	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デフォルト値
//	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
//	// 定数バッファ1番
//	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
//	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
//	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
//	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
//
//	//テクスチャサンプラーの設定
//	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
//	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
//	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
//	samplerDesc.MinLOD = 0.0f;
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
//	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//
//	//ルートシグネチャ
//	ID3D12RootSignature* rootSignature;
//	//ルートシグネチャの設定
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//	rootSignatureDesc.pParameters = rootParams;
//	rootSignatureDesc.NumParameters = _countof(rootParams);
//	rootSignatureDesc.pStaticSamplers = &samplerDesc;
//	rootSignatureDesc.NumStaticSamplers = 1;
//
//	//ルートシグネチャのシリアライズ
//	ID3DBlob* rootSigBlob = nullptr;
//	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
//		&rootSigBlob, &errorBlob);
//	assert(SUCCEEDED(result));
//
//	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
//		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
//	assert(SUCCEEDED(result));
//	rootSigBlob->Release();
//	//パイプラインにルートシグネチャをセット
//	gpipelineDesc.pRootSignature = rootSignature;
//
//	//パイプランステートの生成
//	ID3D12PipelineState* pipelineState = nullptr;
//	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
//	assert(SUCCEEDED(result));
//
//	//インデックスデータ全体のサイズ
//	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
//
//	////リソース設定
//	//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	//resDesc.Width = sizeIB;
//	//resDesc.Height = 1;
//	//resDesc.DepthOrArraySize = 1;
//	//resDesc.MipLevels = 1;
//	//resDesc.SampleDesc.Count = 1;
//	//resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//インデックスバッファの生成
//	ID3D12Resource* indexBuff = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&heapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&resDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&indexBuff));
//
//	//インデックスバッファをマッピング
//	uint16_t* indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
//	//全インデックスに対して
//	for (int i = 0; i < _countof(indices); i++)
//	{
//		//インデックスをコピー
//		indexMap[i] = indices[i];
//	}
//	//マッピング解除
//	indexBuff->Unmap(0, nullptr);
//
//	//インデックスバッファビューの作成
//	D3D12_INDEX_BUFFER_VIEW ibView{};
//	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibView.Format = DXGI_FORMAT_R16_UINT;
//	ibView.SizeInBytes = sizeIB;
//
//	//ここから
//
//	//1つ目
//	TexMetadata metadata{};
//	ScratchImage scratchImg{};
//	//WICテクスチャのコード
//	result = LoadFromWICFile(
//		L"Resources/cube.jpg",
//		WIC_FLAGS_NONE,
//		&metadata,
//		scratchImg
//	);
//
//	ScratchImage mipChain{};
//	//ミップマップ生成
//	result = GenerateMipMaps(
//		scratchImg.GetImages(),
//		scratchImg.GetImageCount(),
//		scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT,
//		0,
//		mipChain);
//	if (SUCCEEDED(result))
//	{
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//
//	//読み込んだディヒューズテクスチャをSRGBとして扱う
//	metadata.format = MakeSRGB(metadata.format);
//
//	//テクスチャヒープ設定
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty =
//		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//	//リソース設定
//	D3D12_RESOURCE_DESC textureResouceDesc{};
//	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResouceDesc.Format = metadata.format;
//	textureResouceDesc.Width = metadata.width;
//	textureResouceDesc.Height = (UINT)metadata.height;
//	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
//	textureResouceDesc.SampleDesc.Count = 1;
//
//	//テクスチャバッファの生成
//	ID3D12Resource* texBuff = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResouceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff));
//
//	//全ミップマップについて
//	for (size_t i = 0; i < metadata.mipLevels; i++)
//	{
//		//ミップマップレベルを指定してイメージを取得
//		const Image* img = scratchImg.GetImage(i, 0, 0);
//		//テクスチャバッファにデータ転送
//		result = texBuff->WriteToSubresource(
//			(UINT)i,
//			nullptr,
//			img->pixels,
//			(UINT)img->rowPitch,
//			(UINT)img->slicePitch);
//		assert(SUCCEEDED(result));
//	}
//
//	//2つ目
//	TexMetadata metadata2{};
//	ScratchImage scratchImg2{};
//	//WICテクスチャのコード
//	result = LoadFromWICFile(
//		L"Resources/reimu.png",
//		WIC_FLAGS_NONE,
//		&metadata2,
//		scratchImg2
//	);
//
//	ScratchImage mipChain2{};
//	//ミップマップ生成
//	result = GenerateMipMaps(
//		scratchImg2.GetImages(),
//		scratchImg2.GetImageCount(),
//		scratchImg2.GetMetadata(),
//		TEX_FILTER_DEFAULT,
//		0,
//		mipChain2);
//	if (SUCCEEDED(result))
//	{
//		scratchImg2 = std::move(mipChain2);
//		metadata2 = scratchImg2.GetMetadata();
//	}
//
//	//読み込んだディヒューズテクスチャをSRGBとして扱う
//	metadata2.format = MakeSRGB(metadata2.format);
//
//	//リソース設定
//	D3D12_RESOURCE_DESC textureResouceDesc2{};
//	textureResouceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResouceDesc2.Format = metadata2.format;
//	textureResouceDesc2.Width = metadata2.width;
//	textureResouceDesc2.Height = (UINT)metadata2.height;
//	textureResouceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
//	textureResouceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
//	textureResouceDesc2.SampleDesc.Count = 1;
//
//	//テクスチャバッファの生成
//	ID3D12Resource* texBuff2 = nullptr;
//	result = dxCommon->GetDevice()->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResouceDesc2,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff2));
//}
//
//void SpriteCommon::Draw()
//{
//	//パイプラインステートとルートシグネチャの設定コマンド
//	dxCommon.GetCommandList()->SetPipelineState(pipelineState);
//	dxCommon.GetCommandList()->SetGraphicsRootSignature(rootSignature);
//
//	//インデックスバッファビューの設定コマンド
//	dxCommon.GetCommandList()->IASetIndexBuffer(&ibView);
//
//	//プリミティブ形状の設定コマンド
//	//三角形リスト
//	dxCommon.GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//頂点バッファビューの設定コマンド
//	dxCommon.GetCommandList()->IASetVertexBuffers(0, 1, vbView.Get());
//
//	//定数バッファビュー(CBV)の設定コマンド
//	dxCommon.GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
//
//	//SRVヒープの設定コマンド
//	dxCommon.GetCommandList()->SetDescriptorHeaps(1, &srvHeap);
//
//	//SRVヒープの先頭ハンドルを取得(SRVを指している)
//	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
//
//	//描画フラグ
//	/*if (draw_flg)
//	{
//		srvGpuHandle.ptr += incrementSize;
//	}*/
//
//	//SRVヒープの先頭にあるSRVをルートパラメータ１番に設定
//	dxCommon.GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
//
//	//描画コマンド
//	//object3ds->DrawObject3d(dxCommon.GetCommandList(), vbView, ibView, _countof(indices));
//}
