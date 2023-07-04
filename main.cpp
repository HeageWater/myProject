#include "object.h"
#include "key.h"
#include "WindowApi.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Controller.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Sound.h"
#include "ObjFile.h"
#include "PostEffect.h"
#include <fstream>
#include "ImguiManager.h"
#include <imgui.h>
#include <wrl.h>

using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//3Dオブジェクトの数
	const size_t kObjectCount = 1;

	//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, 10);		//視点座標
	XMFLOAT3 target(0, 0, 0);		//注意点座標
	XMFLOAT3 up(0, 1, 0);			//上方向ベクトル
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//射影変換行列
	XMMATRIX matProjection;

	//カメラの回転角
	float angle = 0.0f;

	bool draw_flg = false;

	MSG msg{};

	HRESULT result;

	//宣言
	//ComPtr<DirectXCommon> dxCommon = nullptr;
	DirectXCommon* dxCommon = new DirectXCommon();
	dxCommon->Initialize();

	//ComPtr<Key> key = new Key(dxCommon->GetWindow()->GetHInstance(), dxCommon->GetWindow()->GetHwnd());
	Key* key = new Key(dxCommon->GetWindow()->GetHInstance(), dxCommon->GetWindow()->GetHwnd());
	key->Initialize();

	/*ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;*/

	//ComPtr<Controller> controller = nullptr;
	Controller* controller = nullptr;
	controller = Controller::GetInstance();

	//Sound* sound = new Sound();

	//Sound sound2;

	//int bgm = sound->SoundLoadWave("Resource/BGM.wav");
	//int bgm = sound2.SoundLoadWave("Resource//BGM.wav");



	//初期化

	//DirectX初期化処理ここまで

	//描画初期化処理ここから

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 noemal;
		XMFLOAT2 uv;
	};

	//頂点データ
	Vertex vertices[] =
	{
		//	x,		y,		z,		u,	 v
		//前
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f, -5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{  5.0f,  5.0f, -5.0f},{},{1.0f,0.0f}},//右上 

		//後
		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f,  5.0f,  5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//左
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
		{{ -5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//右
		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{  5.0f, -5.0f,  5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//下
		{{  5.0f, -5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f, -5.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f, -5.0f,  5.0f},{},{1.0f,1.0f}},//右下
		{{ -5.0f, -5.0f,  5.0f},{},{1.0f,0.0f}},//右上

		//上
		{{  5.0f,  5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ -5.0f,  5.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{  5.0f,  5.0f,  5.0f},{},{1.0f,1.0f}},//右下
		{{ -5.0f,  5.0f,  5.0f},{},{1.0f,0.0f}},//右上
	};

	//インデックスデータ
	unsigned short indices[] =
	{
		//前
		0,1,2,
		2,1,3,
		//後
		6,5,4,
		7,5,6,
		//左
		8,9,10,
		11,10,9,
		//右
		14,13,12,
		13,14,15,
		//下
		18,17,16,
		17,18,19,
		//上
		20,21,22,
		23,22,21,
	};

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//法線計算
	for (int i = 0; i < sizeof(indices) / sizeof(unsigned short) / 3; i++)
	{
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		//p0→p1ベクトル,p0→p2ベクトルを計算(ベクトルの減算)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		//正規化(長さを１に)
		normal = XMVector3Normalize(normal);

		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].noemal, normal);
		XMStoreFloat3(&vertices[index1].noemal, normal);
		XMStoreFloat3(&vertices[index2].noemal, normal);
	}

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
	result = dxCommon->GetDevice()->CreateCommittedResource(
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
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	//全頂点に対して
	for (int i = 0; i < _countof(vertices); i++)
	{
		//座標コピー
		vertMap[i] = vertices[i];
	}

	//つながりを削除
	vertBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;

	//頂点1つ分のデータサイズ
	//vbView.StrideInBytes = sizeof(XMFLOAT3);
	vbView.StrideInBytes = sizeof(vertices[0]);

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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipelineDesc{};

	//シェーダーの設定
	gpipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	gpipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定

	//ラスタライザの設定

	//カリングしない
	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//ポリゴン内塗りつぶし
	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//深度クリッピングを有効に
	gpipelineDesc.RasterizerState.DepthClipEnable = true;

	//ブレンドステート
	//gpipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;
	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//ブレンドを無効
	blenddesc.BlendEnable = false;

	{
		////ブレンドを有効
		//blenddesc.BlendEnable = true;
		////加算
		//blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		////ソースの値を10%使う
		//blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		////デストの値を0%使う
		//blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		//加算合成
		////加算
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		////ソースの値を10%使う
		//blenddesc.SrcBlend = D3D12_BLEND_ONE;
		////デストの値を0%使う
		//blenddesc.DestBlend = D3D12_BLEND_ZERO;

		////減算
		//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		////ソースの値を10%使う
		//blenddesc.SrcBlend = D3D12_BLEND_ONE;
		////デストの値を0%使う
		//blenddesc.DestBlend = D3D12_BLEND_ONE;

		////色反転
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		////1.0f-デストカラーの値
		//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		////使わない
		//blenddesc.DestBlend = D3D12_BLEND_ZERO;

		////半透明合成
		//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		////ソースのα値
		//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		////1.0f-ソースのα値
		//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}

	//デプスステンシルステートの作成
	gpipelineDesc.DepthStencilState.DepthEnable = true;
	gpipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//RBGA全てのチャンネルを描画
	//ポリゴン内塗りつぶし
	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;

	//ワイヤーフレーム
	//gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	//頂点レイアウトの設定
	gpipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	gpipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	gpipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	//描画対象は一つ
	gpipelineDesc.NumRenderTargets = 1;
	//0~255指定のRGBA
	gpipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//1ピクセルのつき1回サンプリング
	gpipelineDesc.SampleDesc.Count = 1;


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

	ID3D12Resource* constBuffMaterial = nullptr;
	//定数バッファの生成
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される 
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 0.5f);	//普通の色

	constMapMaterial->mat = XMMatrixIdentity();

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメタの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//定数バッファビュー
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//定数バッファ番号
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デフォルト値
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	// 定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	gpipelineDesc.pRootSignature = rootSignature;

	//パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	////リソース設定
	//resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//resDesc.Width = sizeIB;
	//resDesc.Height = 1;
	//resDesc.DepthOrArraySize = 1;
	//resDesc.MipLevels = 1;
	//resDesc.SampleDesc.Count = 1;
	//resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
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

	//ここから

	//1つ目
	TexMetadata metadata{};
	ScratchImage scratchImg{};
	//WICテクスチャのコード
	result = LoadFromWICFile(
		L"Resources/cube.jpg",
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

	//テクスチャヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc{};
	textureResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc.Format = metadata.format;
	textureResouceDesc.Width = metadata.width;
	textureResouceDesc.Height = (UINT)metadata.height;
	textureResouceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResouceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResouceDesc.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
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


	//2つ目
	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	//WICテクスチャのコード
	result = LoadFromWICFile(
		L"Resources/reimu.png",
		WIC_FLAGS_NONE,
		&metadata2,
		scratchImg2
	);

	ScratchImage mipChain2{};
	//ミップマップ生成
	result = GenerateMipMaps(
		scratchImg2.GetImages(),
		scratchImg2.GetImageCount(),
		scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain2);
	if (SUCCEEDED(result))
	{
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	//読み込んだディヒューズテクスチャをSRGBとして扱う
	metadata2.format = MakeSRGB(metadata2.format);

	//リソース設定
	D3D12_RESOURCE_DESC textureResouceDesc2{};
	textureResouceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResouceDesc2.Format = metadata2.format;
	textureResouceDesc2.Width = metadata2.width;
	textureResouceDesc2.Height = (UINT)metadata2.height;
	textureResouceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResouceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResouceDesc2.SampleDesc.Count = 1;

	//テクスチャバッファの生成
	ID3D12Resource* texBuff2 = nullptr;
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResouceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));


	//射影変換
	{
		////ヒープ設定
		//D3D12_HEAP_PROPERTIES cbHeapProp{};
		//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		////リソース設定	
		//D3D12_RESOURCE_DESC cbResouceDesc{};
		//cbResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		//cbResouceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		//cbResouceDesc.Height = 1;
		//cbResouceDesc.DepthOrArraySize = 1;
		//cbResouceDesc.MipLevels = 1;
		//cbResouceDesc.SampleDesc.Count = 1;
		//cbResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		////定数バッファ生成
		//result = device->CreateCommittedResource(
		//	&cbHeapProp,
		//	D3D12_HEAP_FLAG_NONE,
		//	&cbResouceDesc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffTransform0));
		//assert(SUCCEEDED(result));

		//result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0);
		//assert(SUCCEEDED(result));

		////定数バッファ生成Part2
		//result = device->CreateCommittedResource(
		//	&cbHeapProp,
		//	D3D12_HEAP_FLAG_NONE,
		//	&cbResouceDesc,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffTransform1));
		//assert(SUCCEEDED(result));

		//result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1);
		//assert(SUCCEEDED(result));

		////並行投影行列の計算
		//constMapTransform->mat = XMMatrixIdentity();

		//constMapTransform->mat.r[0].m128_f\[0] = 2.0f / window->window_width;
		//constMapTransform->mat.r[1].m128_f32[1] = -2.0f / window->window_height;
		//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
		//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

		////↑のまとめ
		//XMMATRIX newb = XMMatrixOrthographicOffCenterLH
		//(0, window->window_width,
		//	window->window_height,0,
		//	0.0f, 1.0f);

		/*constMapTransform->mat = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)window->window_width / window->window_height,
			0.1f, 1000.0f*/

			//射影変換行列
		matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)dxCommon->GetWindow()->window_width / dxCommon->GetWindow()->window_height,
			0.1f, 1000.0f);

		//matProjection = XMMatrixIdentity();
	}

	//deviceに代入してから入れる
	//ComPtr<Object3ds> object3ds = new Object3ds(dxCommon->GetDevice());
	Object3ds* object3ds = new Object3ds(dxCommon->GetDevice());
	Object3ds* object3ds2 = new Object3ds(dxCommon->GetDevice());

	//全ミップマップについて
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		//ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg2.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
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
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	//テクスチャバッファより下に書く
	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	srvDesc.Format = resDesc.Format;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dxCommon->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

	//ハンドルのサイズ調整
	UINT incrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incrementSize;

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};			//設定構造体
	srvDesc2.Format = textureResouceDesc2.Format;	//RGBA float
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc2.Texture2D.MipLevels = textureResouceDesc2.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	dxCommon->GetDevice()->CreateShaderResourceView(texBuff2, &srvDesc2, srvHandle);

	//サウンド設定
	//sound2.SoundPlayWave(bgm);
	//sound->SoundPlayLoopWave(bgm);

	//描画初期化処理ここまで

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Inilialize(dxCommon);

	Sprite* sprite = new Sprite();
	sprite->Inilialize(spriteCommon);

	sprite->LoadResource();

	PostEffect* postEffect = nullptr;

	postEffect = new PostEffect(dxCommon->GetDevice());
	postEffect->Initialize();

	ImguiManager* imguiManager = new ImguiManager();
	imguiManager->Initialize(dxCommon);

	//ゲームループ1
	while (true)
	{
		//毎フレーム処理ここから

		//キー入力更新
		key->Update();
		controller->Update();

		//postEffect->Update();

		//windowsのメッセージ処理
		if (dxCommon->GetWindow()->ProcessMessege())
		{
			break;
		}

		//描画フラグ切り替え
		if (key->Push(DIK_SPACE))
		{
			draw_flg = !draw_flg;
		}

		//移動速度
		float speed = 0.4f;

		Vector2 vec = { 0,0 };
		vec = controller->GetLeftStickVec();

		float DeadSpace = 0.3f;

		//移動
		if (key->Keep(DIK_UP) || vec.y < -DeadSpace)
		{
			eye.z -= speed;
		}
		if (key->Keep(DIK_DOWN) || vec.y > DeadSpace)
		{
			eye.z += speed;
		}
		if (key->Keep(DIK_RIGHT) || vec.x > DeadSpace)
		{
			object3ds->position.x += speed;
		}
		if (key->Keep(DIK_LEFT) || vec.x < -DeadSpace)
		{
			object3ds->position.x -= speed;
		}

		if (key->Keep(DIK_ESCAPE))
		{
			break;
		}

		if (key->Keep(DIK_D))
		{
			eye.x += 3;		//視点座標
		}
		if (key->Keep(DIK_A))
		{
			eye.x -= 3;		//視点座標
		}
		if (key->Keep(DIK_W))
		{
			eye.y += 3;		//視点座標
		}
		if (key->Keep(DIK_S))
		{
			eye.y -= 3;		//視点座標
		}
		if (key->Keep(DIK_Q))
		{
			eye.z += 3;		//視点座標
		}
		if (key->Keep(DIK_E))
		{
			eye.z -= 3;		//視点座標
		}

		matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		//更新処理
		object3ds->UpdateObject3d(matView, matProjection);
		sprite->Update(matView);

		postEffect->Update(matView, matProjection);

		//ImGuiここから
		imguiManager->Begin();

		ImGui::InputFloat("object3ds->position.x", &object3ds->position.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("sprite->position.x", &sprite->position.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("sprite->position.y", &sprite->position.y, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("sprite->position.z", &sprite->position.z, 0.0f, 10.0f, "%f");

		ImGui::InputFloat("eye->scale.x", &eye.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("eye->scale.y", &eye.y, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("eye->scale.z", &eye.z, 0.0f, 10.0f, "%f");

		ImGui::InputFloat("post->rotation.x", &postEffect->rotation.x, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("post->rotation.y", &postEffect->rotation.y, 0.0f, 10.0f, "%f");
		ImGui::InputFloat("post->rotation.z", &postEffect->rotation.z, 0.0f, 10.0f, "%f");
		//ImGui::Text("Hello, world %d", 123);

		//ImGuiここまで
		imguiManager->End();

		//毎フレーム処理ここまで


		//2.描画先の変更

		//3.画面クリア

		//4.描画処理ここから

		//パイプラインステートとルートシグネチャの設定コマンド
		dxCommon->GetCommandList()->SetPipelineState(pipelineState);
		dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature);

		//インデックスバッファビューの設定コマンド
		dxCommon->GetCommandList()->IASetIndexBuffer(&ibView);

		//プリミティブ形状の設定コマンド
		//三角形リスト
		dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//頂点バッファビューの設定コマンド
		dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

		//定数バッファビュー(CBV)の設定コマンド
		dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		//SRVヒープの設定コマンド
		dxCommon->GetCommandList()->SetDescriptorHeaps(1, &srvHeap);

		//SRVヒープの先頭ハンドルを取得(SRVを指している)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		//SRVヒープの先頭にあるSRVをルートパラメータ１番に設定
		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//DirectXCommon
		dxCommon->PreDraw();

		//描画フラグ
		if (draw_flg)
		{
			//srvGpuHandle.ptr += incrementSize;

			//PostEffect準備
			postEffect->PreDraw(dxCommon->GetCommandList());

			//描画コマンド
			object3ds->DrawObject3d(dxCommon->GetCommandList(), vbView, ibView, _countof(indices));

			//Imgui描画
			imguiManager->Draw(dxCommon);

			//PostEffect終了
			postEffect->PostDraw(dxCommon->GetCommandList());

			//PostEffect描画
			postEffect->Draw(dxCommon->GetCommandList(), pipelineState, rootSignature, ibView);
		}
		else
		{
			//SRVヒープの先頭にあるSRVをルートパラメータ１番に設定
			//この中で画像入れてるからさっさとスプライトに移動
			//dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//描画コマンド
			object3ds->DrawObject3d(dxCommon->GetCommandList(), vbView, ibView, _countof(indices));

			//postEffect->Draw();
			sprite->PreDraw();

			//画像を1に入れたものに
			dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

			//三角形描画
			sprite->Draw(srvGpuHandle);

			//Imgui描画
			imguiManager->Draw(dxCommon);
		}
		//4.描画処理ここまで

		//5.リソースバリア
		dxCommon->PostDraw();

		////描画状態から
	}

	//windowAPI後始末

	//ウィンドウクラスを登録解除
	dxCommon->GetWindow()->Finalize();
	imguiManager->Finalize();

	//元データ解放
	delete key;
	delete object3ds;
	delete dxCommon;
	//delete spriteCommon;
	//delete sprite;
	delete postEffect;
	delete imguiManager;

	return 0;
}