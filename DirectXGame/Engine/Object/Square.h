#pragma once
#include "DirectX.h"
#include "MyMath.h"
#include "GPipeline.h"
#include "VertBuff.h"

class UISquare :public VertBuff
{
private:
	GPipeline pipeline_;
	UINT vertexSize_;
public:
	ScreenVertex pv_[4];
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
	MyDirectX* dx_ = nullptr;

	//パイプライン
	GPipeline* pipeline_ = nullptr;

	//結果
	HRESULT result_;

	//コンストバッファデータトランスフォーム
	struct ConstBufferDataTransform 
	{
		Matrix mat_;
	};

	//トランスフォーム
	ComPtr<ID3D12Resource> transform_;

	//コンストマップ用トランスフォーム
	ConstBufferDataTransform* constMapTransform_ = nullptr;

	//ヒーププロップ
	D3D12_HEAP_PROPERTIES cbHeapProp_{};

	//リソースデスク
	D3D12_RESOURCE_DESC cbResourceDesc_{};

	//ヒーププロップ
	D3D12_HEAP_PROPERTIES heapProp_{};

	//リソースデスク
	D3D12_RESOURCE_DESC resourceDesc_{};

	//コンストバッファデータマテリアル
	struct ConstBufferDataMaterial
	{
		Vector4D color_;	//	RGBA
	};

	//マテリアル
	ComPtr<ID3D12Resource> material_;

	//マップ用マテリアル
	ConstBufferDataMaterial* mapMaterial_ = nullptr;

	//頂点
	Vertex pv_[4];

	//頂点サイズ
	UINT vertexSize_;

	//インデックスサイズ
	UINT indexSize_;

	//インデックス
	unsigned short indices_[6];
public:

	//モデル
	MyMath::ObjMatrix obj_;

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

