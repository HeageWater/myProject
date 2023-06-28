#pragma once
#include "Sprite.h"
using namespace Microsoft::WRL;

class  PostEffect : Sprite
{
public:
	PostEffect(DirectXCommon* dxCommon);
	PostEffect(SpriteCommon* spriteCommon);
	~PostEffect();

	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	void PostDraw(ID3D12GraphicsCommandList* cmdList);

	void Draw(ID3D12GraphicsCommandList* cmdList);
	void Initialize();

	static const float clearColor[4];
private:
	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	DirectXCommon* dxCommon_;
};