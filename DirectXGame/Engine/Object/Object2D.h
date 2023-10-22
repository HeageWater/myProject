#pragma once
#include "GPipeline.h"
#include "VertBuff.h"

/// <summary>
/// 2Dモデル
/// </summary>
class Object2D:public VertBuff
{

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
	void SetTransform(Vector3D pos) { trans_ = pos; }

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	//カメラ
	Matrix matWorld_;

	//カメラscale
	Matrix matScale_;
	//scale
	Vector3D scale_;

	//カメラrot
	Matrix matRot_;
	//rot
	Vector3D rotAngle_;

	//カメラtrans
	Matrix matTrans_;
	//trans
	Vector3D trans_;

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

	//パイプライン
	GPipeline pipeline_;

	//頂点サイズ
	uint32_t vertexSize_;

	//頂点サイズ(複数)
	std::vector<VertexObj> vertices_;

	//インデックスサイズ
	UINT indexSize_;

	//インデックス
	std::vector<unsigned short> indices_;
};

