#pragma once
#include "Sprite.h"

class PEffect : Sprite
{
public:
	PEffect(SpriteCommon* spriteCommon, DirectXCommon* dxCommon);

	void Draw();
	void Initialize();
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

private:
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//Sprite sprite_;
};