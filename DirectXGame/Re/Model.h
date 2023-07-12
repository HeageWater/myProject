#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertBuff.h"

class Model :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	MyDirectX* dx = nullptr;
	GPipeline* pipeline = nullptr;
	struct ConstBufferDataTransform {
		Matrix mat;
	};
	ComPtr<ID3D12Resource> transform;
	ConstBufferDataTransform* constMapTransform = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	D3D12_RESOURCE_DESC cbResourceDesc{};

	UINT vertexSize;
	std::vector<Vertex> vertices;
	//UINT indexSize;
	//std::vector<unsigned short> indices;
public:
	MyMath::ObjMatrix mat;

	void Initialize(Shader shader, const char* filename);
	void Initialize(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_);
public:
	Model();
	Model(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_);
	void MatUpdate(Matrix matView, Matrix matProjection);
	void Draw(size_t handle);
private:
	void SetVertices() override;
};

