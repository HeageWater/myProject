#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

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

	//windowAPI初期化処理ここから

	//ウィンドウサイズ
	const int window_width = 1280;
	const int window_height = 720;

	//ウィンドウクラスの設定
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = L"DirectXGame";
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	//ウィンドウサイズ{x,y,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };

	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(w.lpszClassName, //クラス名
		L"DirectXGame",						  //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,				  //標準的なウィンドウスタイル
		CW_USEDEFAULT,						  //表示x座標(OSに任せる)
		CW_USEDEFAULT,						  //表示y座標(OSに任せる)
		wrc.right - wrc.left,				  //ウィンドウ横幅
		wrc.bottom - wrc.top,				  //ウィンドウ縦幅
		nullptr,							  //親ウィンドウハンドル
		nullptr,							  //メニューハンドル
		w.hInstance,						  //呼び出しアプリケーション
		nullptr);							  //オプション

	//ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	//windowAPI初期化処理ここまで

	//directx初期化処理ここから

	//OutputDebugStringA("Hello,DirectX!!\n");

	MSG msg{};

	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;

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
		commandQueue, hwnd, &swapChainDesc, nullptr, nullptr,
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
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
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
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	//DirectX初期化処理ここまで

	//描画初期化処理ここから

	//頂点データ
	XMFLOAT3 vertices[] =
	{
		{-0.5f,-0.5f,0.0f},//左下
		{-0.5f,+0.5f,0.0f},//左上
		{+0.5f,-0.5f,0.0f},//右下
	};

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

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
	XMFLOAT3* vertMap = nullptr;
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
	vbView.StrideInBytes = sizeof(XMFLOAT3);

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
			0,DXGI_FORMAT_R32G32B32_FLOAT,
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
	gpipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//ポリゴン内塗りつぶし
	gpipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//深度クリッピングを有効に
	gpipelineDesc.RasterizerState.DepthClipEnable = true;

	//ブレンドステート
	gpipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL;
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

	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
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

	//描画初期化処理ここまで

	//ゲームループ1
	while (true)
	{
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

		//毎フレーム処理ここから

		//キーボード情報の取得開始
		keyboard->Acquire();

		//全キーの入力状態を保存する
		BYTE key[256] = {};
		keyboard->GetDeviceState(sizeof(key), key);

		if (key[DIK_0])
		{
			OutputDebugStringA("Hit 0\n");
		}

		//毎フレーム処理ここまで

		//グラフィックスコマンド

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
		commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		//3.画面クリア

		//青っぽい色
		FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };

		if (key[DIK_SPACE])
		{
			//赤っぽい色
			clearColor[0] = { 0.75f };
		}

		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		//4.描画処理ここから

		//パイプラインステートとルートシグネチャの設定コマンド
		commandList->SetPipelineState(pipelineState);
		commandList->SetGraphicsRootSignature(rootSignature);

		//プリミティブ形状の設定コマンド
		//三角形リスト
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//頂点バッファビューの設定コマンド
		commandList->IASetVertexBuffers(0, 1, &vbView);

		//ビューポート設定コマンド
		D3D12_VIEWPORT viewport[2][2]{};
		float w = window_width / 2;
		float h = window_height / 2;

		//ビューポート設定コマンドを、コマンドリストに積む
		//commandList->RSSetViewports(1, &viewport[i][j]);

		D3D12_RECT scissorRect[2][2]{};
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				scissorRect[i][j].left = w * i;							 //切り抜き座標左
				scissorRect[i][j].right = scissorRect[i][j].left + w;	 //切り抜き座標右
				scissorRect[i][j].top = h * j;							 //切り抜き座標上
				scissorRect[i][j].bottom = scissorRect[i][j].top + h;	 //切り抜き座標下

				viewport[i][j].Width = scissorRect[i][j].right;
				viewport[i][j].Height = scissorRect[i][j].bottom;
				viewport[i][j].TopLeftX = -100;
				viewport[i][j].TopLeftY = 100;
				viewport[i][j].MinDepth = 0.0f;
				viewport[i][j].MaxDepth = 1.0f;

				//シザー矩形設定コマンドを、コマンドリストに積む
				commandList->RSSetScissorRects(1, &scissorRect[i][j]);

				//ビューポート設定コマンドを、コマンドリストに積む
				commandList->RSSetViewports(1, &viewport[i][j]);

				//描画コマンド
				//全ての頂点を使って描画
				commandList->DrawInstanced(_countof(vertices), 1, 0, 0);
			}
		}

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
	UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}