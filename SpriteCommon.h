#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <vector>
#include <fstream>
using namespace DirectX;

#pragma comment(lib,"d3dcompiler.lib")

class SpriteCommon 
{
public:
	SpriteCommon();
	~SpriteCommon();
	void Inilialize(DirectXCommon* dxCommon);
	void Draw();
	DirectXCommon dxCommon;

	HRESULT result;
	Microsoft::WRL::ComPtr<D3D12_VERTEX_BUFFER_VIEW> vbView{};
};