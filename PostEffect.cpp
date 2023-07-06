#include "PostEffect.h"
#include "WindowApi.h"
#include "Vector3.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

float Vnum = 0;

PostEffect::PostEffect(ID3D12Device* device)
{
	//dxCommon_ = dxCommon;

	this->device = device;

	for (size_t i = 0; i < 4; i++)
	{
		matWorld.r[i] = { 0,0,0 };
	}
}

PostEffect::PostEffect(Sprite* sprite)
{
	this->sprite = sprite;
}

PostEffect::~PostEffect()
{

}

void PostEffect::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//CD3DX12_RESOURCE_BARRIER* barrier;
	//dxCommon_->GetCommandList()

	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	cmdList->ResourceBarrier(1,
		&trans);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewPort = {};
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = WindowApi::window_width;
	viewPort.Height = WindowApi::window_height;

	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect = {};
	rect.left = (long)0.0f;
	rect.top = (long)0.0f;
	rect.right = WindowApi::window_width;
	rect.bottom = WindowApi::window_height;

	cmdList->RSSetScissorRects(1, &rect);

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDraw(ID3D12GraphicsCommandList* cmdList)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	cmdList->ResourceBarrier(1, &barrier);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList, ID3D12PipelineState* pipelineState, ID3D12RootSignature* rootSignature, D3D12_INDEX_BUFFER_VIEW& ibView)
{
	//三角形リスト
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	////各種コマンドリスト

	ID3D12DescriptorHeap* srvHeap = descHeapSRV.Get();

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };


	//定数バッファにデータ転送

	//
	cmdList->SetPipelineState(pipelineStateP.Get());
	cmdList->SetGraphicsRootSignature(rootSignatureP.Get());

	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//SRVヒープの先頭にあるSRVをルートパラメーター1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);


	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	////定数バッファビューの設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	cmdList->DrawInstanced(_countof(indices), 1, 0, 0);
	//cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void PostEffect::Draw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	spriteCommon_->dxCommon_->GetCommandList()->SetPipelineState(spriteCommon_->pipelineState);
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootSignature(spriteCommon_->rootSignature);

	//プリミティブ形状の設定コマンド
	//三角形リスト
	spriteCommon_->dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファビューの設定コマンド
	spriteCommon_->dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//描画コマンド
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void PostEffect::Update(XMMATRIX& matView, XMMATRIX& matProjection)
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

	XMMATRIX matProjection1 = XMMatrixIdentity();

	////射影変換行列
	//matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),
	//	(float)spriteCommon_->dxCommon_->GetWindow()->window_width / spriteCommon_->dxCommon_->GetWindow()->window_height,
	//	0.1f, 1000.0f);

	//↑のまとめ
	matProjection1 = XMMatrixOrthographicOffCenterLH
	(0,
		(float)WindowApi::Get()->window_width,
		(float)WindowApi::Get()->window_height,
		0,
		0.0f,
		1.0f);

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	constMapTransform->mat = this->matWorld * matProjection1;

	constBuffTransform->Unmap(0, nullptr);
}

void PostEffect::Initialize()
{
	HRESULT result;

	/*for (size_t i = 0; i < 4; i++)
	{
		matWorld.r[i] = { 0,0,0 };
	}*/

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowApi::window_width,
		(UINT)WindowApi::window_height,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_HEAP_PROPERTIES  properities = {};
	properities.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	properities.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	properities.Type = D3D12_HEAP_TYPE_CUSTOM;

	CD3DX12_CLEAR_VALUE value = {};
	value.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	value.Color[0] = clearColor[0];
	value.Color[1] = clearColor[1];
	value.Color[2] = clearColor[2];
	value.Color[3] = clearColor[3];

	result = device->CreateCommittedResource(
		&properities,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&value,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	const UINT pixelCount = WindowApi::window_width * WindowApi::window_height;

	const UINT rowPitch = sizeof(UINT) * WindowApi::window_width;

	const UINT deathPitch = rowPitch * WindowApi::window_height;

	UINT* img = new UINT[pixelCount];

	for (size_t i = 0; i < pixelCount; i++)
	{
		img[i] = 0xff0000ff;
	}

	result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, deathPitch);
	assert(SUCCEEDED(result));
	delete[] img;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvDescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		texBuff.Get()
		, &srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc = {};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//設定を元にSRV用のデスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDeac{};
	renderTargetViewDeac.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDeac.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(texBuff.Get(), &renderTargetViewDeac, descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,
			WindowApi::window_width,
			WindowApi::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_HEAP_PROPERTIES prote = {};
	prote.Type = D3D12_HEAP_TYPE_DEFAULT;

	CD3DX12_CLEAR_VALUE value2 = {};

	value2.Format = DXGI_FORMAT_D32_FLOAT;
	value2.DepthStencil.Depth = 1.0f;
	value2.DepthStencil.Stencil = 0;

	result = device->CreateCommittedResource(
		&prote,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&value2,
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));

	//dsv用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	//struct Vertex
	//{
	//	XMFLOAT3 pos;
	//	XMFLOAT2 uv;
	//};

	//頂点データ
	Vertex vertices[4] =
	{
		//	x,		y,		z,		u,	 v
		//前
		{{ 5.0f,   5.0f, -5.0f},{},{0.0f,1.0f}},//左下
		{{ 5.0f,  10.0f, -5.0f},{},{0.0f,0.0f}},//左上
		{{ 10.0f,  5.0f, -5.0f},{},{1.0f,1.0f}},//右下
		{{ 10.0f, 10.0f, -5.0f},{},{1.0f,0.0f}},//右上 
	};

	unsigned short indices[] =
	{
		//前
		0,1,2,
		2,1,3,
	};

	//頂点データサイズ　= 頂点データサイズ一つ分 * 要素数
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	Vnum = _countof(indices);

	prote.Type = D3D12_HEAP_TYPE_UPLOAD;

	CD3DX12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Buffer(sizeof(Vertex));
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		//ヒープ設定
		&prote,
		D3D12_HEAP_FLAG_NONE,
		//リソース設定
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));


	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeof(Vertex) * 4;

	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);

	//定数バッファ
	ID3D12Resource* constBuffMaterial = nullptr;

	prote.Type = D3D12_HEAP_TYPE_UPLOAD;
	resourceDesc.Buffer((sizeof(constBuffMaterial) + 0xff) & ~0xff);

	//ヒープの設定
	D3D12_HEAP_PROPERTIES heapProp{};

	//GPUへの転送用
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		//ヒープ設定
		&prote,
		D3D12_HEAP_FLAG_NONE,
		//リソース設定
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	matWorld = XMMatrixIdentity();

	//転送
	result = constBuffTransform->Map(0, nullptr, (void**)&this->constMapTransform);
	assert(SUCCEEDED(result));

	////単位行列
	//matWorld = XMMatrixIdentity();
	//constBuffMaterial->Unmap(0, nullptr);	

	//パイプライン生成
	CreateGraphicsPipelineState();
}

void PostEffect::CreateGraphicsPipelineState()
{
	HRESULT result = S_FALSE;

	//頂点シェーダオブジェクト
	ComPtr<ID3DBlob> vsBlob;

	//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;

	//エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob;


	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"PostEffectVS.hlsl",//シェーダファイル名
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

		return;
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"PostEffectPS.hlsl",//シェーダファイル名
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

		return;
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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスクの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定

	//ラスタライザ
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//カリングしない
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//デプスステンシルステートの作成
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	//レンダーターゲット
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};

	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	//描画対象は一つ
	gpipeline.NumRenderTargets = 1;
	//0~255指定のRGBA
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//1ピクセルのつき1回サンプリング
	gpipeline.SampleDesc.Count = 1;

	//デスクリプタレンジの設定
	CD3DX12_DESCRIPTOR_RANGE descriptorSRV;

	descriptorSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメタの設定
	CD3DX12_ROOT_PARAMETER rootParams[2] = {};
	rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &descriptorSRV, D3D12_SHADER_VISIBILITY_ALL);

	//
	CD3DX12_STATIC_SAMPLER_DESC sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);

	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	//ルートシグネチャ
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	//ルートシグネチャの設定
	rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &sampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSignatureBlob;

	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSignatureBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
		rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignatureP));
	assert(SUCCEEDED(result));

	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootSignatureP.Get();

	//生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineStateP));
	assert(SUCCEEDED(result));
}
