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

class Square :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	MyDirectX* dx = nullptr;
	GPipeline* pipeline = nullptr;

	HRESULT result_;

	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};

	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;

	Vertex pv[4];
	UINT vertexSize;
	UINT indexSize;
	unsigned short indices[6];
public:
	MyMath::ObjMatrix obj;

public:
	void Initialize(GPipeline* pipeline_, Shader shader, size_t blendMord = GPipeline::NONE_BLEND);
	Square(GPipeline* pipeline_, Shader shader, size_t blendMord = GPipeline::NONE_BLEND);
	Square();
	void MatUpdate(Matrix matView, Matrix matProjection, Matrix billboard = Matrix());
	void MatUpdate(Matrix matView, Matrix matProjection, float a, Matrix billboard = Matrix());
	void Draw(size_t handle);
private:
	void SetVertices() override;
};

