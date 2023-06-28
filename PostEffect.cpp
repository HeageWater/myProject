#include "PostEffect.h"
#include "WindowApi.h"
#include "Vector3.h"
#include <d3dx12.h>

using namespace DirectX;

const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

PostEffect::PostEffect(DirectXCommon* dxCommon)
{
	dxCommon_ = dxCommon;
}

PostEffect::PostEffect(SpriteCommon* spriteCommon) :Sprite()
{
	spriteCommon_ = spriteCommon;
}

PostEffect::~PostEffect()
{
}

void PostEffect::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER* barrier;

	cmdList->ResourceBarrier(1,
		&barrier->Transition(texBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WindowApi::window_width, WindowApi::window_height));

	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WindowApi::window_width, WindowApi::window_height));

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDraw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//spriteCopy

	//ワールド座標の更新
	//定数バッファにデータ転送

	cmdList->SetPipelineState(spriteCommon_->pipelineState);

	cmdList->SetGraphicsRootSignature(spriteCommon_->rootSignature);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//各種コマンドリスト

	//spriteCommon_->Draw();

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };

	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

}

void PostEffect::Initialize()
{
	HRESULT result;


	CD3DX12_RESOURCE_DESC texresDesx = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowApi::window_width,
		(UINT)WindowApi::window_height,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesx,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor),
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

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvDescHeapDesc.NumDescriptors = 1;

	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	spriteCommon_->dxCommon_->GetDevice()->CreateShaderResourceView(
		texBuff.Get()
		, &srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc = {};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//設定を元にSRV用のデスクリプタヒープを生成
	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDeac{};
	renderTargetViewDeac.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDeac.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	dxCommon_->GetDevice()->CreateRenderTargetView(texBuff.Get(), &renderTargetViewDeac, descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//リソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,
			WindowApi::window_width,
			WindowApi::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	result = dxCommon_->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	result = dxCommon_->GetDevice()->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dxCommon_->GetDevice()->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());

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
		{{ -0.5f, -0.5f, -0.0f},{},{0.0f,1.0f}},//左下
		{{ -0.5f,  0.5f, -0.0f},{},{0.0f,0.0f}},//左上
		{{  0.5f, -0.5f, -0.0f},{},{1.0f,1.0f}},//右下
		{{  0.5f,  0.5f, -0.0f},{},{1.0f,0.0f}},//右上 
	};

	//頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		//ヒープ設定
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		//リソース設定
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	Vertex* vertMap = nullptr;
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
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeof(Vertex) * 4;

	//頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(Vertex);

	//定数バッファ
	ID3D12Resource* constBuffMaterial = nullptr;
	//定数バッファの生成
	result = dxCommon_->GetDevice()->CreateCommittedResource(
		//ヒープ設定
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		//リソース設定
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(constBuffMaterial) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

}