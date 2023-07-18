#pragma once
#include <d3d12.h>
#include "MyMath.h"
#include <wrl.h>

class ConstBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};


	struct ConstBufferDataMaterial {
		Vector4D color;	//	RGBA
	};
	struct ConstBufferTimeMaterial {
		float time;
	};
	ComPtr<ID3D12Resource> material;
	ConstBufferDataMaterial* mapMaterial = nullptr;
	ComPtr<ID3D12Resource> timeMaterial;
	ConstBufferTimeMaterial* timeMapMaterial = nullptr;
	float timer = 100000;
public:
	ConstBuff(ID3D12Device* dev, const float winwidth, const float winheight);
	~ConstBuff();
	void Initialize(ID3D12Device* dev, const float winwidth, const float winheight);
	void ChangeColor(const Vector4D& color);
	void TimeUpdate();
	void Update(ID3D12GraphicsCommandList* cmdList);
	void SetBufferView(ID3D12GraphicsCommandList* cmdList);
};

