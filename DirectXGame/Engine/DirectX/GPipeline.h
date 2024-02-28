#pragma once
#include "Shader.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>

/// <summary>
/// パイプラインクラス
/// </summary>
class GPipeline
{
public:
	enum BlendMord
	{
		NONE_BLEND = 0,
		ADD_BLEND,
		SUB_BLEND,
		INV_BLEND,
		ALPHA_BLEND,
	};

private:

	/// <summary>
	/// シェーダーセット
	/// </summary>
	/// <param name="shader"></param>
	void SetShader(Shader shader);

	/// <summary>
	/// ブレンドモード
	/// </summary>
	/// <param name="blenddesc"></param>
	/// <param name="mord"></param>
	void Blend(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, const int mord = NONE_BLEND);

	/// <summary>
	/// ルートパラメータセット
	/// </summary>
	/// <param name="rootParam"></param>
	/// <param name="type"></param>
	/// <param name="shaderRegister"></param>
	/// <param name="registerSpace"></param>
	/// <param name="shaderVisibility"></param>
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

	/// <summary>
	/// ルートパラメータセット
	/// </summary>
	/// <param name="rootParam"></param>
	/// <param name="type"></param>
	/// <param name="shaderRegister"></param>
	/// <param name="registerSpace"></param>
	/// <param name="shaderVisibility"></param>
	void SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescriptorRange, UINT numDescriptorRanges, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

	/// <summary>
	/// ルートシグネチャセット
	/// </summary>
	/// <param name="rootParamNum"></param>
	void SetRootSignature(ID3D12Device* dev);

	/// <summary>
	/// スクリーンルートシグネチャセット
	/// </summary>
	/// <param name="dev"></param>
	void SetScreenRootSignature(ID3D12Device* dev);

public:
	GPipeline();
	GPipeline(ID3D12Device* dev, Shader shader, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullmord = D3D12_CULL_MODE_BACK);
	GPipeline(ID3D12Device* dev, Shader shader);
	//GPipeline(D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, ID3D12Device* dev, Shader shader);
	GPipeline(ID3D12Device* dev, Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="topologyType"></param>
	/// <param name="fillmord"></param>
	/// <param name="cullmord"></param>
	void Initialize(ID3D12Device* dev, Shader shader, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullmord = D3D12_CULL_MODE_BACK);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="inputLayout"></param>
	/// <param name="inputLayoutSize"></param>
	/// <param name="topologyType"></param>
	/// <param name="fillmord"></param>
	/// <param name="cullmord"></param>
	void Init(ID3D12Device* dev, Shader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE fillmord = D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE cullmord = D3D12_CULL_MODE_BACK);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="primitive"></param>
	void Update(ID3D12GraphicsCommandList* cmdList, D3D_PRIMITIVE_TOPOLOGY primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/// <summary>
	/// コマンドリストセット
	/// </summary>
	/// <param name="cmdList"></param>
	void Setting(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ブレンドセット
	/// </summary>
	/// <param name="mord"></param>
	void SetBlend(ID3D12Device* dev, int mord);

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	ComPtr<ID3D12PipelineState> state_;
	HRESULT result_;
};