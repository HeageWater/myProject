#pragma once

#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

//頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 noemal;
	XMFLOAT2 uv;
};

class Vertexs
{
public:
	Vertexs();
	//頂点データ
	Vertex vertices[];
};