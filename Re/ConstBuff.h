#pragma once
#include <d3d12.h>
#include "MyMath.h"
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
class ConstBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	HRESULT result_;
	D3D12_HEAP_PROPERTIES heapProp_{};
	D3D12_RESOURCE_DESC resourceDesc_{};


	struct ConstBufferDataMaterial {
		Vector4D color_;	//	RGBA
	};
	struct ConstBufferTimeMaterial {
		float time_;
	};
	ComPtr<ID3D12Resource> material_;
	ConstBufferDataMaterial* mapMaterial_ = nullptr;
	ComPtr<ID3D12Resource> timeMaterial_;
	ConstBufferTimeMaterial* timeMapMaterial_ = nullptr;
	float timer_ = 100000;
public:
	ConstBuff(ID3D12Device* dev, const size_t winwidth, const size_t winheight);
	~ConstBuff();
	void ChangeColor(const Vector4D& color);
	void TimeUpdate();
	void Update(ID3D12GraphicsCommandList* cmdList);
	void SetBufferView(ID3D12GraphicsCommandList* cmdList);
};

