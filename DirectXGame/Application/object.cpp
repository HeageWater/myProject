//#include "object.h"
//
//struct ConstBufferDataMaterial
//{
//	XMFLOAT4 color;
//};
//
//struct ConstBufferDataTransform
//{
//	XMMATRIX mat;
//};
//
////������
//Object3ds::Object3ds(ID3D12Device* device)
//{
//	HRESULT result;
//
//	//�萔�o�b�t�@�̃q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{};
//	//GPU�ւ̓]���p
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//
//	//�萔�o�b�t�@�̃��\�[�X�ݒ�
//	D3D12_RESOURCE_DESC resdesc{};
//	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
//	resdesc.Height = 1;
//	resdesc.DepthOrArraySize = 1;
//	resdesc.MipLevels = 1;
//	resdesc.SampleDesc.Count = 1;
//	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//�萔�o�b�t�@�̐���
//	result = device->CreateCommittedResource(
//		&heapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&resdesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&this->constBuffTransform));
//	assert(SUCCEEDED(result));
//
//	//�萔�o�b�t�@�̃}�b�s���O
//	result = this->constBuffTransform->Map(0, nullptr, (void**)&this->constMapTransform);
//	assert(SUCCEEDED(result));
//}
//
////�X�V����
//void Object3ds::UpdateObject3d(XMMATRIX& matView, XMMATRIX& matProjection)
//{
//	XMMATRIX matScale, matRot, matTrans;
//
//	//�X�P�[���Ȃǂ̌v�Z
//	matScale = XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z);
//	matRot = XMMatrixIdentity();
//	matRot *= XMMatrixRotationZ(this->rotation.z);
//	matRot *= XMMatrixRotationX(this->rotation.x);
//	matRot *= XMMatrixRotationY(this->rotation.y);
//
//	matTrans = XMMatrixTranslation(this->position.x, this->position.y, this->position.z);
//
//	this->matWorld = XMMatrixIdentity();
//	this->matWorld *= matScale;
//	this->matWorld *= matRot;
//	this->matWorld *= matTrans;
//
//	//�f�[�^�]��
//	this->constMapTransform->mat = this->matWorld * matView * matProjection;
//}
//
////�`��
//void Object3ds::DrawObject3d(
//	ID3D12GraphicsCommandList* commandList,
//	D3D12_VERTEX_BUFFER_VIEW& vbView,
//	D3D12_INDEX_BUFFER_VIEW& ibView, 
//	UINT numIndices)
//{
//	//���_�o�b�t�@�̐ݒ�
//	commandList->IASetVertexBuffers(0, 1, &vbView);
//
//	//�C���f�b�N�X�o�b�t�@�̐ݒ�
//	commandList->IASetIndexBuffer(&ibView);
//
//	//�萔�o�b�t�@�r���[�̐ݒ�R�}���h
//	commandList->SetGraphicsRootConstantBufferView(2, this->constBuffTransform->GetGPUVirtualAddress());
//
//	//�`��R�}���h
//	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
//}