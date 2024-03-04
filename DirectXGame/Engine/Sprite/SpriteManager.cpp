#include "SpriteManager.h"
using namespace DirectX;

#pragma comment(lib,"d3dcompiler.lib")

SpriteCommon::SpriteCommon()
{
	result_ = {};
}

SpriteCommon::~SpriteCommon()
{
	//delete dxCommon_;
}

void SpriteCommon::Inilialize(MyDirectX* dxCommon,bool isSemiArpha)
{
	assert(dxCommon);

	dxCommon_ = dxCommon;

	//頂点シェーダオブジェクト
	ID3DBlob* vsBlob = nullptr;

	//ピクセルシェーダオブジェクト
	ID3DBlob* psBlob = nullptr;

	//エラーオブジェクト
	ID3DBlob* errorBlob = nullptr;

	//頂点シェーダの読み込みとコンパイル
	result_ = D3DCompileFromFile(
		L"Resources/shader/SpriteVS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードを可能にする
		"main", "vs_5_0",//エントリーポインタ名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&vsBlob, &errorBlob);

	//エラーなら
	if (FAILED(result_)) {
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
	result_ = D3DCompileFromFile(
		L"Resources/shader/SpritePS.hlsl",//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードを可能にする
		"main", "ps_5_0",//エントリーポインタ名、シェーダモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//デバック用設定
		0,
		&psBlob, &errorBlob);

	//エラーなら
	if (FAILED(result_)) {
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
	pipelineDesc_.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//標準設定

	//ラスタライザの設定

	//カリングしない
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//ポリゴン内塗りつぶし
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//深度クリッピングを有効に
	pipelineDesc_.RasterizerState.DepthClipEnable = false;

	//ブレンドステート
	pipelineDesc_.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];

	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	if (!isSemiArpha)
	{
		//ブレンドを無効
		blenddesc.BlendEnable = false;
	}
	else
	{
		//ブレンドを有効
		blenddesc.BlendEnable = true;
		//加算
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		//ソースの値を10%使う
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		//デストの値を0%使う
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

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

		//半透明合成
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		//ソースのα値
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		//1.0f-ソースのα値
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	}


	//図形の形状設定
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//頂点レイアウトの設定
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

	//その他の設定
	//描画対象は一つ
	pipelineDesc_.NumRenderTargets = 1;
	//0~255指定のRGBA
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//1ピクセルのつき1回サンプリング
	pipelineDesc_.SampleDesc.Count = 1;



	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.RegisterSpace = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメタの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};

	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;					//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;						//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//定数バッファビュー
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			//定数バッファ番号
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//デフォルト値
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える
	//// 定数バッファ1番
	//rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//定数バッファビュー
	//rootParams[2].Descriptor.ShaderRegister = 1;					//定数バッファ番号
	//rootParams[2].Descriptor.RegisterSpace = 0;						//デフォルト値
	//rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//全てのシェーダーから見える

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

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	result_ = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result_));

	//ルートシグネチャ
	//ID3D12RootSignature* rootSignature;

	result_ = dxCommon_->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result_));
	rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	pipelineDesc_.pRootSignature = rootSignature_;

	//パイプランステートの生成
	//ID3D12PipelineState* pipelineState = nullptr;

	result_ = dxCommon_->GetDev()->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result_));

	//verticesCount = _countof(vertices);
}

void SpriteCommon::Draw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	dxCommon_->GetCmdList()->SetPipelineState(pipelineState_);
	dxCommon_->GetCmdList()->SetGraphicsRootSignature(rootSignature_);

	//プリミティブ形状の設定コマンド
	//三角形リスト
	dxCommon_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファビューの設定コマンド
	dxCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &vbView_);

	//描画コマンド
	dxCommon_->GetCmdList()->DrawInstanced(verticesCount_, 1, 0, 0);
	//dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
}