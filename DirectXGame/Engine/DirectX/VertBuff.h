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
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
};

struct VertexObj
{
	Vector3D pos;
	Vector3D normal;
	Vector2D uv;
	Vector4D color;
};

struct ScreenVertex
{
	Vector3D pos;
	Vector2D uv;
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
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//Buffer
	ComPtr<ID3D12Resource> vertBuff;

	//index
	bool ibExist = false;

	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;

	//デスクリプタヒープ
	D3D12_RESOURCE_DESC resDesc{};

	//ヒーププロップ
	D3D12_HEAP_PROPERTIES heapProp{};

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
	D3D12_RESOURCE_DESC ResDesc() { return resDesc; }
};

