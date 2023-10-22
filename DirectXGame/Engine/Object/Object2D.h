#pragma once
#include "GPipeline.h"
#include "VertBuff.h"

/// <summary>
/// 2Dモデル
/// </summary>
class Object2D:public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//カメラ
	Matrix matWorld;

	//カメラscale
	Matrix matScale;
	//scale
	Vector3D scale;

	//カメラrot
	Matrix matRot;
	//rot
	Vector3D rotAngle;

	//カメラtrans
	Matrix matTrans;
	//trans
	Vector3D trans;

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

	//パイプライン
	GPipeline pipeline;

	//頂点サイズ
	uint32_t vertexSize;

	//頂点サイズ(複数)
	std::vector<VertexObj> vertices;

	//インデックスサイズ
	UINT indexSize;

	//インデックス
	std::vector<unsigned short> indices;

private:

	/// <summary>
	/// 頂点セット
	/// </summary>
	void SetVertices() override;

	/// <summary>
	/// scaleセット
	/// </summary>
	void SetMatScaling();

	/// <summary>
	/// rotationセット
	/// </summary>
	void SetMatRotation();

	/// <summary>
	/// transformセット
	/// </summary>
	void SetMatTransform();
public:
	Object2D(ID3D12Device* dev, Shader shader, UINT vertexNum, float rad = 0.5f);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void MatUpdate(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// posをtransformにセット
	/// </summary>
	/// <param name="pos"></param>
	void SetTransform(Vector3D pos) { trans = pos; }
};

