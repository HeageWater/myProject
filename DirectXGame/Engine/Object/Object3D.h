#pragma once
#include "MyMath.h"
#include "GPipeline.h"
#include "DirectX.h"
#include "VertBuff.h"
#include <d3d12.h>
#include <wrl.h>

/// <summary>
/// 3Dモデル
/// </summary>
class Object3D :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//DirectX
	MyDirectX* dx_ = nullptr;

	//パイプライン
	GPipeline* pipeline_ = nullptr;
public:

	//matrix
	MyMath::ObjMatrix mat_;

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

	//頂点サイズ
	UINT vertexSize_;

	//頂点サイズ(複数)
	std::vector<Vertex> vertices_;

	//インデックスサイズ
	UINT indexSize_;

	//インデックス(複数)
	std::vector<unsigned short> indices_;

	//親子関係
	Object3D* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	void Initialize(Shader shader);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dx_"></param>
	/// <param name="pipeline_"></param>
	/// <param name="shader"></param>
	void Initialize(MyDirectX* dx_, GPipeline* pipeline_, Shader shader);
public:
	Object3D();
	Object3D(MyDirectX* dx_, GPipeline* pipeline_, Shader shader);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void MatUpdate(Matrix matView, Matrix matProjection);

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

