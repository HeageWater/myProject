#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertBuff.h"

/// <summary>
/// モデルクラス
/// </summary>
class Model :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	//DirectX
	MyDirectX* dx = nullptr;

	//パイプライン
	GPipeline* pipeline = nullptr;

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

	//頂点
	std::vector<Vertex> vertices;

public:

	//matrix
	MyMath::ObjMatrix mat;

	//matView
	Matrix View_;

	//matProdaction
	Matrix Prodaction_;

	/// <summary>
	/// 初期化(主にこれを使う)
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="filename"></param>
	void Initialize(Shader shader, const char* filename);

	/// <summary>
	/// 初期化(DirextXを使うとき)
	/// </summary>
	/// <param name="dx_"></param>
	/// <param name="pipeline_"></param>
	void Initialize(MyDirectX* dx_, GPipeline* pipeline_);

	/// <summary>
	/// 初期化(全部必要な時)
	/// </summary>
	/// <param name="dx_"></param>
	/// <param name="shader"></param>
	/// <param name="filename"></param>
	/// <param name="pipeline_"></param>
	void Initialize(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_);

public:
	Model();
	Model(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_);

	/// <summary>
	/// matrix更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void MatUpdate(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="handle"></param>
	void Draw(size_t handle);

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="View"></param>
	/// <param name="Projection"></param>
	void SetCamera(Matrix View, Matrix Projection);
private:

	/// <summary>
	/// 頂点セット
	/// </summary>
	void SetVertices() override;
};

