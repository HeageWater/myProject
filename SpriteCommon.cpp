#include "SpriteCommon.h"
#include "Vertex.h"

SpriteCommon::SpriteCommon()
{
	result = {};
}

SpriteCommon::~SpriteCommon()
{
	delete dxCommon_;
}

void SpriteCommon::Inilialize(DirectXCommon* dxCommon)
{
	////dxCommon_ = new DirectXCommon();
	////dxCommon_->Initialize();

	assert(dxCommon);

	dxCommon_ = dxCommon;

	////頂点データ
	//Vertex vertices[] =
	//{
	//	//	x,		y,		z,		u,	 v
	//	//前
	//	{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
	//	{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
	//	{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//右下
	//	{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//右上 

	//	//後
	//	{{ -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},//左下
	//	{{ -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},//左上
	//	{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
	//	{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

	//	//左
	//	{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
	//	{{ -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
	//	{{ -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
	//	{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

	//	//右
	//	{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
	//	{{  5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
	//	{{  5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
	//	{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

	//	//下
	//	{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
	//	{{ -5.0f, -5.0f, -5.0f},{},{0.0f,0.0f}},//左上
	//	{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
	//	{{ -5.0f, -5.0f,  5.0f},{},{1.0f,0.0f}},//右上

	//	//上
	//	{{  5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},//左下
	//	{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
	//	{{  5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},//右下
	//	{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上
	//};

	Vector3 vertices[] =
	{
		{-0.5f,-0.5f,0.0f},
		{-0.5f,+0.5f,0.0f},
		{+0.5f,-0.5f,0.0f}
	};
	
	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	//UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	UINT sizeVB = static_cast<UINT>(sizeof(Vector3) * _countof(vertices));

	//頂点バッファの設定
	//ヒープの設定
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPUへの転送用
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = dxCommon_->GetDevice()->CreateCommittedResource(
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
	//Vertex* vertMap = nullptr;
	Vector3* vertMap = nullptr;
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

	// 頂点バッファビューの作成
	//D3D12_VERTEX_BUFFER_VIEW vbView{};

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;

	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vector3);
	//vbView.StrideInBytes = sizeof(vertices[0]);

	//頂点シェーダオブジェクト
	ID3DBlob* vsBlob = nullptr;

	//ピクセルシェーダオブジェクト
	ID3DBlob* psBlob = nullptr;

	//エラーオブジェクト
	ID3DBlob* errorBlob = nullptr;

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードを可能にする
		"main", "vs_5_0",//エントリーポインタ名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob, &errorBlob);

	//エラーなら
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードを可能にする
		"main", "ps_5_0",//エントリーポインタ名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob, &errorBlob);

	//エラーなら
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},

		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},

		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};


	//グラフィックスパイプライン設定
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定

	//ラスタライザの設定

	//カリングしない
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//深度クリッピングを有効に
	pipelineDesc.RasterizerState.DepthClipEnable = true;

	//ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL;

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	//描画対象は一つ
	pipelineDesc.NumRenderTargets = 1;
	//0~255指定のRGBA
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//1ピクセルのつき1回サンプリング
	pipelineDesc.SampleDesc.Count = 1;


	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	//ルートシグネチャ
	//ID3D12RootSignature* rootSignature;

	result = dxCommon_->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;

	//パイプランステートの生成
	//ID3D12PipelineState* pipelineState = nullptr;

	result = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	verticesCount = _countof(vertices);
}

void SpriteCommon::Draw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	//dxCommon_->GetCommandList()->SetPipelineState(pipelineState);
	//dxCommon_->GetCommandList()->SetGraphicsRootSignature(rootSignature);

	//プリミティブ形状の設定コマンド
	//三角形リスト
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファビューの設定コマンド
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//描画コマンド
	dxCommon_->GetCommandList()->DrawInstanced(verticesCount, 1, 0, 0);
	//dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}