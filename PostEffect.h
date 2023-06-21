#pragma once
#include "Sprite.h"
using namespace Microsoft::WRL;

class  PostEffect : Sprite
{
public:
	PostEffect();
	~PostEffect();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Initialize();
private:
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};