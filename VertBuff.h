#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <stdint.h>
#include "Vector2.h"
#include "Vector3.h"
#include <wrl.h>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
};

struct VertexObj
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
	//Vector4D color;
};

struct ScreenVertex
{
	Vector3 pos;
	Vector2 uv;
};

class VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	HRESULT result;
protected:
	//	vertex
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> vertBuff;

	//	index
	bool ibExist = false;
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> indexBuff;

	D3D12_RESOURCE_DESC resDesc{};
	D3D12_HEAP_PROPERTIES heapProp{}; // ÉqÅ[Évê›íË

private:
	void SetResDesc(UINT size);
	void BuffTransferGPU(ID3D12Resource* buff, ID3D12Device* dev);

protected:
	void VBInitialize(ID3D12Device* dev, UINT sizeVB, UINT vertSize, UINT sizeIB = NULL, uint16_t* indices = nullptr, UINT indicesSize = NULL);
	void VertBuffUpdate(ID3D12GraphicsCommandList* cmdList);
public:
	VertBuff();
	
	virtual void SetLighting();

	virtual void SetVertices();

	//	Getter
	D3D12_RESOURCE_DESC ResDesc() { return resDesc; }
};

