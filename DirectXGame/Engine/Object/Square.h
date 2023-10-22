#pragma once
#include "DirectX.h"
#include "MyMath.h"
#include "GPipeline.h"
#include "VertBuff.h"

class UISquare :public VertBuff
{
private:
	GPipeline pipeline;
	UINT vertexSize;
public:
	ScreenVertex pv[4];
	UISquare(ID3D12Device* dev, Shader shader);
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE handle);
private:
	void SetVertices() override;
};

/// <summary>
/// 板ポリ
/// </summary>
class Square :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX
	MyDirectX* dx = nullptr;

	//パイプライン
	GPipeline* pipeline = nullptr;

	//結果
	HRESULT result_;

	//コンストバッファデータトランスフォーム
	struct ConstBufferDataTransform 
	{
		Matrix mat;
	};

	//トランスフォーム
	ComPtr<ID3D12Resource> transform;

	//コンストマップ用トランスフォーム
	ConstBufferDataTransform* constMapTransform = nullptr;

	//ヒーププロップ
	D3D12_HEAP_PROPERTIES cbHeapProp{};

	//リソースデスク
	D3D12_RESOURCE_DESC cbResourceDesc{};

	//ヒーププロップ
	D3D12_HEAP_PROPERTIES heapProp{};

	//リソースデスク
	D3D12_RESOURCE_DESC resourceDesc{};

	//コンストバッファデータマテリアル
	struct ConstBufferDataMaterial
	{
		Vector4D color;	//	RGBA
	};

	//マテリアル
	ComPtr<ID3D12Resource> material;

	//マップ用マテリアル
	ConstBufferDataMaterial* mapMaterial = nullptr;

	//頂点
	Vertex pv[4];

	//頂点サイズ
	UINT vertexSize;

	//インデックスサイズ
	UINT indexSize;

	//インデックス
	unsigned short indices[6];
public:

	//モデル
	MyMath::ObjMatrix obj;

public:

	Square(GPipeline* pipeline_, Shader shader, size_t blendMord = GPipeline::NONE_BLEND);
	Square();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pipeline_"></param>
	/// <param name="shader"></param>
	/// <param name="blendMord"></param>
	void Initialize(GPipeline* pipeline_, Shader shader, size_t blendMord = GPipeline::NONE_BLEND);
	
	/// <summary>
	/// 更新(ビルボード)
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="billboard"></param>
	void MatUpdate(Matrix matView, Matrix matProjection, Matrix billboard = Matrix());

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="a">TransXを減らす値</param>
	/// <param name="billboard"></param>
	void MatUpdate(Matrix matView, Matrix matProjection, float a, Matrix billboard = Matrix());

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="handle"></param>
	void Draw(size_t handle);
private:

	/// <summary>
	/// 頂点をセット
	/// </summary>
	void SetVertices() override;
};

