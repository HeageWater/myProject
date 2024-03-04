#include "GPipeline.h"
#include "DirectX.h"
#include <vector>
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

GPipeline::GPipeline(ID3D12Device* dev, Shader shader)
{
	D3D12_DESCRIPTOR_RANGE range = {};
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range.BaseShaderRegister = 0;
	range.NumDescriptors = 1;

	D3D12_ROOT_PARAMETER rp[2] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[0].DescriptorTable.pDescriptorRanges = &range;
	rp[0].DescriptorTable.NumDescriptorRanges = 1;

	SetRootParam(rp[1], D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0);

	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.NumParameters = _countof(rp);
	rsDesc.NumStaticSamplers = 1;

#pragma region sampler
	//	テクスチャーサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC sampler{};
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.MinLOD = 0.0f;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
#pragma endregion

	rsDesc.pStaticSamplers = &sampler;
	rsDesc.pParameters = rp;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> rsBlob;
	ComPtr<ID3DBlob> errBlob;

	result_ = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		rsBlob.ReleaseAndGetAddressOf(),
		errBlob.ReleaseAndGetAddressOf());

	result_ = dev->CreateRootSignature(
		0,
		rsBlob.Get()->GetBufferPointer(),
		rsBlob.Get()->GetBufferSize(),
		IID_PPV_ARGS(rootSignature_.ReleaseAndGetAddressOf()));

	SetShader(shader);
	pipelineDesc_.DepthStencilState.DepthEnable = false;
	pipelineDesc_.DepthStencilState.StencilEnable = false;

	D3D12_INPUT_ELEMENT_DESC layout[2] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	pipelineDesc_.InputLayout.NumElements = _countof(layout);
	pipelineDesc_.InputLayout.pInputElementDescs = layout;
#pragma region  Blending
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blend(blenddesc);
#pragma endregion
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc_.NumRenderTargets = 1;
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
#pragma region Rasterizer
	// 設定
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面カリング
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
#pragma endregion
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	pipelineDesc_.SampleDesc.Count = 1;
	pipelineDesc_.SampleDesc.Quality = 0;
	pipelineDesc_.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	pipelineDesc_.pRootSignature = rootSignature_.Get();
	result_ = dev->CreateGraphicsPipelineState(&pipelineDesc_,
		IID_PPV_ARGS(state_.ReleaseAndGetAddressOf()));
}

GPipeline::GPipeline(ID3D12Device* dev, Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord)
{
	Init(dev, shader, inputLayout, inputLayoutSize, topologyType, fillmord);
}

void GPipeline::Update(ID3D12GraphicsCommandList* cmdList, D3D_PRIMITIVE_TOPOLOGY primitive)
{
	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(state_.Get());
	cmdList->IASetPrimitiveTopology(primitive);
}

void GPipeline::Setting(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetGraphicsRootSignature(rootSignature_.Get());
}

void GPipeline::Init(ID3D12Device* dev, Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize,
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord, D3D12_CULL_MODE cullmord)
{
	// シェーダーの設定
	SetShader(shader);

	// サンプルマスクの設定
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

#pragma region Rasterizer
	// 設定
	pipelineDesc_.RasterizerState.CullMode = cullmord; // 背面カリング
	pipelineDesc_.RasterizerState.FillMode = fillmord; // ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
#pragma endregion

#pragma region  Blending
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blend(blenddesc);

#pragma endregion

	// 頂点レイアウトの設定
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc_.InputLayout.NumElements = inputLayoutSize;

	// 図形の形状設定
	pipelineDesc_.PrimitiveTopologyType = topologyType;

	// その他の設定
	pipelineDesc_.NumRenderTargets = 1;							  // 描画対象は1つ
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc_.SampleDesc.Count = 1;							  // 1ピクセルにつき1回サンプリング

	//	デプスステンシルステート設定
	pipelineDesc_.DepthStencilState.DepthEnable = true;								//	深度テストを行う
	pipelineDesc_.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;		//	書き込み許可
	pipelineDesc_.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;			//	小さければ合格
	pipelineDesc_.DSVFormat = DXGI_FORMAT_D32_FLOAT;									//	深度フォーマット

	SetRootSignature(dev);

	// パイプラインにルートシグネチャをセット
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	// パイプランステートの生成
	result_ = dev->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&state_));
	assert(SUCCEEDED(result_));
}

void GPipeline::Blend(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, const int mord)
{
	//	共通設定
	if (mord != NONE_BLEND) {
		pipelineDesc_.BlendState.AlphaToCoverageEnable = true;
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	}
	else {
		pipelineDesc_.BlendState.AlphaToCoverageEnable = false;
	}

	switch (mord)
	{
	case ADD_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case SUB_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case INV_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blenddesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case ALPHA_BLEND:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		break;
	}
}

void GPipeline::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;							//	定数バッファビュー
	rootParam.Descriptor.ShaderRegister = shaderRegister;	//	定数バッファ番号
	rootParam.Descriptor.RegisterSpace = registerSpace;		//	デフォルト値
	rootParam.ShaderVisibility = shaderVisibility;			//	すべてのシェーダから見る
}

void GPipeline::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescriptorRange, UINT numDescriptorRanges, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;
	rootParam.DescriptorTable.pDescriptorRanges = &pDescriptorRange;
	rootParam.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
	rootParam.ShaderVisibility = shaderVisibility;
}

void GPipeline::SetRootSignature(ID3D12Device* dev)
{
	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma region	ルートパラメータ
	//ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParams[4] = {};
	//ルートパラメータの設定
	//行列
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//陰影
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[1].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	rootParams[1].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//色
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[2].Descriptor.ShaderRegister = 2;					//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダから見える

	//テクスチャレジスタ3番
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//種類
	rootParams[3].DescriptorTable.pDescriptorRanges = &descriptorRange;			//デスクリプタレンジ
	rootParams[3].DescriptorTable.NumDescriptorRanges = 1;						//デスクリプタレンジ数
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//全てのシェーダから見える
#pragma endregion

#pragma region sampler
	//	テクスチャーサンプラーの設定
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
#pragma endregion

#pragma region ルートシグネチャ
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;						//	先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);				//	ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(result_));
	result_ = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result_));
#pragma endregion
}

void GPipeline::SetScreenRootSignature(ID3D12Device* dev)
{
	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma region	ルートパラメータ
	//	ルートパラメータの設定
	std::vector<D3D12_ROOT_PARAMETER> rootParams = {};
	rootParams.resize(1);
	SetRootParam(rootParams[0], D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, descriptorRange, 1, D3D12_SHADER_VISIBILITY_PIXEL);
#pragma endregion

#pragma region sampler
	//	テクスチャーサンプラーの設定
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
#pragma endregion

#pragma region ルートシグネチャ
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();						//	先頭アドレス
	rootSignatureDesc.NumParameters = (int32_t)rootParams.size();				//	ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		rootSigBlob.ReleaseAndGetAddressOf(),
		errorBlob.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result_));
	result_ = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSignature_.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result_));
	// パイプラインにルートシグネチャをセット
	pipelineDesc_.pRootSignature = rootSignature_.Get();
#pragma endregion
}

GPipeline::GPipeline()
{
	result_ = S_OK;
}

//GPipeline::GPipeline(ID3D12Device* dev, Shader shader, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord, D3D12_CULL_MODE cullmord)
//{
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
//		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
//		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
//		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
//	};
//
//	Init(dev, shader, inputLayout, _countof(inputLayout), topologyType, fillmord, cullmord);
//}

void GPipeline::Initialize(ID3D12Device* dev, Shader shader, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord, D3D12_CULL_MODE cullmord)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyz座標
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	法線ベクトル
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uv座標
	};

	Init(dev, shader, inputLayout, _countof(inputLayout), topologyType, fillmord, cullmord);
}

void GPipeline::SetBlend(ID3D12Device* dev, int mord)
{
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blend(blenddesc, mord);
	result_ = dev->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&state_));
	assert(SUCCEEDED(result_));
}

void GPipeline::SetShader(Shader shader)
{
#pragma region VertexShader
	pipelineDesc_.VS.pShaderBytecode = shader.VSBlob()->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = shader.VSBlob()->GetBufferSize();
#pragma endregion
#pragma region HShader
	if (shader.HSBlob() != nullptr) {
		pipelineDesc_.HS.pShaderBytecode = shader.HSBlob()->GetBufferPointer();
		pipelineDesc_.HS.BytecodeLength = shader.HSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region DShader
	if (shader.DSBlob() != nullptr) {
		pipelineDesc_.DS.pShaderBytecode = shader.DSBlob()->GetBufferPointer();
		pipelineDesc_.DS.BytecodeLength = shader.DSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region GShader
	if (shader.GSBlob() != nullptr) {
		pipelineDesc_.GS.pShaderBytecode = shader.GSBlob()->GetBufferPointer();
		pipelineDesc_.GS.BytecodeLength = shader.GSBlob()->GetBufferSize();
	}
#pragma endregion
#pragma region PixcelShader
	pipelineDesc_.PS.pShaderBytecode = shader.PSBlob()->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = shader.PSBlob()->GetBufferSize();
#pragma endregion
}
