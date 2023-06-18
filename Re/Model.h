#pragma once
#include "MyMath.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "VertBuff.h"

class Model :public VertBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
	MyDirectX* dx_ = nullptr;
	GPipeline* pipeline_ = nullptr;
	struct ConstBufferDataTransform {
		Matrix mat_;
	};
	ComPtr<ID3D12Resource> transform_;
	ConstBufferDataTransform* constMapTransform_ = nullptr;
	D3D12_HEAP_PROPERTIES cbHeapProp_{};
	D3D12_RESOURCE_DESC cbResourceDesc_{};

	UINT vertexSize_;
	std::vector<Vertex> vertices_;
	//UINT indexSize_;
	//std::vector<unsigned short> indices_;
public:
	MyMath::ObjMatrix mat_;

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

