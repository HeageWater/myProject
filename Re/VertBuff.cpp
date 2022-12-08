#include "VertBuff.h"
#include <wrl.h>
#include <vector>

void VertBuff::VertBuffUpdate(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	if (ibExist) {
		cmdList->IASetIndexBuffer(&ibView);
	}
}

void VertBuff::VBInitialize(ID3D12Device* dev, UINT sizeVB, UINT vertSize, UINT sizeIB, uint16_t* indices, UINT indicesSize)
{
	SetLighting();

	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	SetResDesc(sizeVB);

	result = dev->CreateCommittedResource(
		&heapProp,					
		D3D12_HEAP_FLAG_NONE,
		&resDesc,							
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	vbView.SizeInBytes = sizeVB;

	SetVertices();

	if (indices != nullptr) {
		ibExist = true;

		SetResDesc(sizeIB);
		result = dev->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc, 
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));
		assert(SUCCEEDED(result));

		uint16_t* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		assert(SUCCEEDED(result));

		for (int i = 0; i < indicesSize; i++) {
			indexMap[i] = indices[i];
		}

		indexBuff->Unmap(0, nullptr);

		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;
	}
}

void VertBuff::SetResDesc(UINT size)
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = size;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void VertBuff::BuffTransferGPU(ID3D12Resource* buff, ID3D12Device* dev)
{
	result = dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));
}
