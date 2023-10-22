#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <stdint.h>
#include "MyMath.h"
#include <wrl.h>
#include <vector>

struct Vertex
{
	Vector3D pos_;
	Vector3D normal_;
	Vector2D uv_;
};

struct VertexObj
{
	Vector3D pos_;
	Vector3D normal_;
	Vector2D uv_;
	Vector4D color_;
};

struct ScreenVertex
{
	Vector3D pos_;
	Vector2D uv_;
};

/// <summary>
/// バッファクラス
/// </summary>
class VertBuff
{
private:

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:

	//vertex
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	//Buffer
	ComPtr<ID3D12Resource> vertBuff_;

	//index
	bool ibExist_ = false;

	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;

	//デスクリプタヒープ
	D3D12_RESOURCE_DESC resDesc_{};

	//ヒーププロップ
	D3D12_HEAP_PROPERTIES heapProp_{};

	//結果
	HRESULT result_;

private:

	/// <summary>
	/// リソースデスクをセット
	/// </summary>
	/// <param name="size"></param>
	void SetResDesc(UINT size);

	/// <summary>
	/// GPUバッファ
	/// </summary>
	/// <param name="buff"></param>
	/// <param name="dev"></param>
	void BuffTransferGPU(ID3D12Resource* buff, ID3D12Device* dev);

protected:

	/// <summary>
	/// ViewBuffをセット
	/// </summary>
	/// <param name="dev"></param>
	/// <param name="sizeVB"></param>
	/// <param name="sizeIB"></param>
	/// <param name="indices"></param>
	/// <param name="indicesSize"></param>
	void VBInitialize(ID3D12Device* dev, UINT sizeVB, UINT sizeIB = NULL, uint16_t* indices = nullptr, UINT indicesSize = NULL);
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cmdList"></param>
	void VertBuffUpdate(ID3D12GraphicsCommandList* cmdList);
public:
	VertBuff();

	/// <summary>
	/// ライトニングセット
	/// </summary>
	virtual void SetLighting();

	/// <summary>
	/// 頂点セット
	/// </summary>
	virtual void SetVertices();

	//	Getter
	D3D12_RESOURCE_DESC ResDesc() { return resDesc_; }
};

