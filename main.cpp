//#include <DirectXMath.h>
//#include <DirectXTex.h>
//#include <d3d12.h>
//#include <dxgi1_6.h>
//#include <vector>
//#include <string>
#include "object.h"
#include "key.h"
#include "WindowApi.h"
////#include "Definition.h"
//using namespace DirectX;
//
//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
//
//#include <d3dcompiler.h>
//#pragma comment(lib,"d3dcompiler.lib")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//初期化
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	//3Dオブジェクトの数
	const size_t kObjectCount = 1;
	//3Dオブジェクトの配列
	//Object3d object3ds[kObjectCount];

	//ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);		//視点座標
	XMFLOAT3 target(0, 0, 0);		//注意点座標
	XMFLOAT3 up(0, 1, 0);			//上方向ベクトル
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	//射影変換行列
	XMMATRIX matProjection;

	////スケ-リング倍率
	//XMFLOAT3 scale = { 1.0f,2.0f,1.0f };
	////回転角
	//XMFLOAT3 rotation = { 0.0f,XM_PI / 4.0f,0.0f };
	////座標
	//XMFLOAT3 position = { 0.0f,0.0f,0.0f };

	//カメラの回転角
	float angle = 0.0f;

	bool draw_flg = false;

	//windowAPI初期化処理ここから

	WindowApi* window = new WindowApi();

	//windowAPI初期化処理ここまで

	//directx初期化処理ここから

	MSG msg{};

	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

	//宣言
	Key* key = new Key(window->w, window->hwnd);

	ID3D12Resource* constBuffTransform0 = nullptr;
	ConstBufferDataTransform* constMapTransform0 = nullptr;

	ID3D12Resource* constBuffTransform1 = nullptr;
	ConstBufferDataTransform* constMapTransform1 = nullptr;

	//DXGIファクトリーの作成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//アダプターの列挙用
	std::vector<IDXGIAdapter4*> adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter4* tmdAdapter = nullptr;

	//パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmdAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		//動的配列に追加する
		adapters.push_back(tmdAdapter);
	}

	//妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		//アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		//ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//デバイスを採用してループを抜ける
			tmdAdapter = adapters[i];
			break;
		}
	}

	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmdAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	//コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	assert(SUCCEEDED(result));

	//コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;

	//色情報
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//マルチサンプルしない
	swapChainDesc.SampleDesc.Count = 1;

	//バックバッファ用
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;

	//バッファ数を2つに設定
	swapChainDesc.BufferCount = 2;

	//フリップ後は破棄
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//スワップチェーンの生成	
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, window->hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain);
	assert(SUCCEEDED(result));

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	//レンダーターゲットビュー
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//裏表の2つ
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;

	//デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	//バックバッファ
	std::vector<ID3D12Resource*> backBuffers;
	backBuffers.resize(swapChainDesc.BufferCount);

	//レンダーターゲットビュー
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		//スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));

		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

		//裏か表でアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		//レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		//シェーダーの計算結果SRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		//レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}

	//フェンスの生成
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		window->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard,
		&keyboard, NULL);

	//入力データ形式のセット
	//標準形式
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		window->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

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
		////x,	y,		z,		u,	v
		//{{-0.4f,-0.7f,0.0f},{0.0f,1.0f}},//左下
		//{{-0.4f,+0.7f,0.0f},{0.0f,0.0f}},//左上
		//{{+0.4f,-0.7f,0.0f},{1.0f,1.0f}},//右下
		//{{+0.4f,+0.7f,0.0f},{1.0f,0.0f}},//右上

		////x,	y,		z,		u,	v
		//{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},//左下
		//{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},//左上
		//{{100.0f,100.0f,0.0f},{1.0f,1.0f}},//右下
		//{{100.0f,  0.0f,0.0f},{1.0f,0.0f}},//右上

		//x,	y,		z,		u,	 v

		//前
		{{ -5.0f, -5.0f, -5.0f},{}, {0.0f,1.0f}},//左下
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
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
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
	result = device->CreateCommittedResource(
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


	//リソース設定
	D3D12_RESOURCE_DESC depthResouceDesc{};
	depthResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResouceDesc.Width = window->window_width;
	depthResouceDesc.Height = window->window_height;
	depthResouceDesc.DepthOrArraySize = 1;
	depthResouceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResouceDesc.SampleDesc.Count = 1;
	depthResouceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

	//リソース設定
	ID3D12Resource* depthBuff = nullptr;
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResouceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff, &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());

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
	result = device->CreateCommittedResource(
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

	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	gpipelineDesc.pRootSignature = rootSignature;

	//パイプランステートの生成
	ID3D12PipelineState* pipelineState = nullptr;
	result = device->CreateGraphicsPipelineState(&gpipelineDesc, IID_PPV_ARGS(&pipelineState));
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
	result = device->CreateCommittedResource(
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
	result = device->CreateCommittedResource(
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
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResouceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));


	//関数化しろぉぉぉぉぉぉぉぉ
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
			(float)window->window_width / window->window_height,
			0.1f, 1000.0f);
	}

	//deviceに代入してから入れる
	Object3ds* object3ds = new Object3ds(device);

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
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));

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
	device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);


	//ハンドルのサイズ調整
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incrementSize;

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};			//設定構造体
	srvDesc2.Format = textureResouceDesc2.Format;	//RGBA float
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc2.Texture2D.MipLevels = textureResouceDesc2.MipLevels;

	//ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff2, &srvDesc2, srvHandle);

	//ここまで

	//描画初期化処理ここまで

	//ゲームループ1
	while (true)
	{
		//毎フレーム処理ここから

		//キー入力更新
		key->Update();

		//ウィンドウメッセージ処理

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//xボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT)
		{
			break;
		}

		//描画フラグ切り替え
		if (key->Push(DIK_SPACE))
		{
			draw_flg = !draw_flg;
		}

		//横回転

		//angle += XMConvertToRadians(1.0f);

		////angleラジアンだけY軸回りい回転。半径は-100
		//eye.y = -100 * sinf(angle);
		//eye.z = -100 * cosf(angle);
		//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));


		//移動速度
		float speed = 0.4f;

		//移動
		if (key->Keep(DIK_UP))
		{
			object3ds[0].position.y += speed;
		}
		if (key->Keep(DIK_DOWN))
		{
			object3ds[0].position.y -= speed;
		}
		if (key->Keep(DIK_RIGHT))
		{
			object3ds[0].position.x += speed;
		}
		if (key->Keep(DIK_LEFT))
		{
			object3ds[0].position.x -= speed;
		}

		//更新処理
		object3ds->UpdateObject3d(matView, matProjection);

		//キーボード情報の取得開始
		keyboard->Acquire();

		//全キーの入力状態を保存する
		BYTE key[256] = {};
		keyboard->GetDeviceState(sizeof(key), key);

		//毎フレーム処理ここまで

		//バックバッファの番号を取得(0番と1番)
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

		//1.リソースバリアで書き込み可能に変更する
		D3D12_RESOURCE_BARRIER barrierDesc{};

		//バックバッファを指定
		barrierDesc.Transition.pResource = backBuffers[bbIndex];

		//表示状態から
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;

		//描画状態へ
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		commandList->ResourceBarrier(1, &barrierDesc);

		//2.描画先の変更
		//レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

		rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		//commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		//3.画面クリア

		//背景
		FLOAT clearColor[] = { 0.0f,0.1f,0.5f,1.0f };

		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		//4.描画処理ここから

		//ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};

		viewport.Width = window->window_width;
		viewport.Height = window->window_height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		//ビューポート設定コマンドを、コマンドリストに積む
		commandList->RSSetViewports(1, &viewport);

		D3D12_RECT scissorRec{};
		scissorRec.left = 0;				 //切り抜き座標左
		scissorRec.right = window->window_width;	 //切り抜き座標右
		scissorRec.top = 0;				 //切り抜き座標上
		scissorRec.bottom = window->window_height;	 //切り抜き座標下

		//シザー矩形設定コマンドを、コマンドリストに積む
		commandList->RSSetScissorRects(1, &scissorRec);

		//パイプラインステートとルートシグネチャの設定コマンド
		commandList->SetPipelineState(pipelineState);
		commandList->SetGraphicsRootSignature(rootSignature);

		//インデックスバッファビューの設定コマンド
		commandList->IASetIndexBuffer(&ibView);

		//プリミティブ形状の設定コマンド
		//三角形リスト
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//頂点バッファビューの設定コマンド
		commandList->IASetVertexBuffers(0, 1, &vbView);

		//定数バッファビュー(CBV)の設定コマンド
		commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		//SRVヒープの設定コマンド
		commandList->SetDescriptorHeaps(1, &srvHeap);

		//SRVヒープの先頭ハンドルを取得(SRVを指している)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		//描画フラグ
		if (draw_flg)
		{
			srvGpuHandle.ptr += incrementSize;
		}

		//SRVヒープの先頭にあるSRVをルートパラメータ１番に設定
		commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		//描画コマンド
		object3ds->DrawObject3d(commandList, vbView, ibView, _countof(indices));

		//4.描画処理ここまで

		//5.リソースバリア

		//描画状態から
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;

		//表示状態に
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		//命令のクローズ
		result = commandList->Close();
		assert(SUCCEEDED(result));

		//コマンドリストの実行
		ID3D12CommandList* commandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, commandLists);

		//画面表示するバッファをフリップ(裏表の入れ替え)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));

		//コマンドの実行完了を待つ
		commandQueue->Signal(fence, ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		//キューをクリア
		result = cmdAllocator->Reset();
		assert(SUCCEEDED(result));

		//再びコマンドリストをためる準備
		result = commandList->Reset(cmdAllocator, nullptr);
		assert(SUCCEEDED(result));
	}

	//windowAPI後始末

	//ウィンドウクラスを登録解除
	UnregisterClass(window->w.lpszClassName, window->w.hInstance);

	//元データ解放
	//delete[] imageData;
	delete key;
	delete window;
	delete object3ds;

	return 0;
}