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
	MyDirectX* dx = nullptr;

	//パイプライン
	GPipeline* pipeline = nullptr;
public:

	//matrix
	MyMath::ObjMatrix mat;

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

	//頂点サイズ
	UINT vertexSize;

	//頂点サイズ(複数)
	std::vector<Vertex> vertices;

	//インデックスサイズ
	UINT indexSize;

	//インデックス(複数)
	std::vector<unsigned short> indices;

	//親子関係
	Object3D* parent = nullptr;

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

