//#pragma once
//#include <DirectXMath.h>
//#include <DirectXTex.h>
//#include <d3d12.h>
//#include <dxgi1_6.h>
//#include <vector>
//#include <string>
//using namespace DirectX;
//
//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
//
//#include <d3dcompiler.h>
//#pragma comment(lib,"d3dcompiler.lib")
//
//class Object3ds
//{
//public:
//	//�萔�o�b�t�@
//	ID3D12Resource* constBuffTransform;
//	//�萔�o�b�t�@�}�b�v(�s��p)
//	ConstBufferDataTransform* constMapTransform;
//	//�A�t�B���ϊ����
//	XMFLOAT3 scale = { 1,1,1 };
//	XMFLOAT3 rotation = { 0,0,0 };
//	XMFLOAT3 position = { 0,0,0 };
//	//���[���h�ϊ��s��
//	XMMATRIX matWorld;
//
//	Object3ds(ID3D12Device* device);
//	void UpdateObject3d(XMMATRIX& matView, XMMATRIX& matProjection);
//	void DrawObject3d(
//		ID3D12GraphicsCommandList* commandList,
//		D3D12_VERTEX_BUFFER_VIEW& vbView,
//		D3D12_INDEX_BUFFER_VIEW& ibView,
//		UINT numIndices);
//};