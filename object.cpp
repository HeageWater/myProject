#include "object.h"

//初期化
Object3ds::Object3ds(ID3D12Device* device)
{
	HRESULT result;

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	//GPUへの転送用
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->constBuffTransform));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = this->constBuffTransform->Map(0, nullptr, (void**)&this->constMapTransform);
	assert(SUCCEEDED(result));
}

//更新処理
void Object3ds::UpdateObject3d(XMMATRIX& matView, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	//スケールなどの計算
	matScale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(this->rotation.z);
	matRot *= XMMatrixRotationX(this->rotation.x);
	matRot *= XMMatrixRotationY(this->rotation.y);

	matTrans = XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	this->matWorld = XMMatrixIdentity();
	this->matWorld *= matScale;
	this->matWorld *= matRot;
	this->matWorld *= matTrans;

	//データ転送
	this->constMapTransform->mat = this->matWorld * matView * matProjection;
}

//描画
void Object3ds::DrawObject3d(
	ID3D12GraphicsCommandList* commandList,
	D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, 
	UINT numIndices)
{
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vbView);

	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&ibView);

	//定数バッファビューの設定コマンド
	commandList->SetGraphicsRootConstantBufferView(2, this->constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}