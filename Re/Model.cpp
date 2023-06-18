#include "Model.h"
#include "ObjFile.h"

void Model::Initialize(Shader shader, const char* filename)
{
	HRESULT newresult;

	D3D12_HEAP_PROPERTIES newheapProp{};
	D3D12_RESOURCE_DESC resourceDesc{};
	//	�q�[�v�ݒ�
	cbHeapProp_.Type = D3D12_HEAP_TYPE_UPLOAD;	//	GPU�]���p

	//	���\�[�X�ݒ�
	cbResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc_.Width = (sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF;
	cbResourceDesc_.Height = 1;
	cbResourceDesc_.DepthOrArraySize = 1;
	cbResourceDesc_.MipLevels = 1;
	cbResourceDesc_.SampleDesc.Count = 1;
	cbResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	����
	newresult = dx_->GetDev()->CreateCommittedResource(
		&cbHeapProp_,	//	�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc_,	//	���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&transform_));
	assert(SUCCEEDED(newresult));

	//	�萔�o�b�t�@�̃}�b�s���O
	newresult = transform_->Map(0, nullptr, (void**)&constMapTransform_);	//	�}�b�s���O
	assert(SUCCEEDED(newresult));

	ObjFile objfile(filename, vertices_);
	vertexSize_ = (UINT)vertices_.size();
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * vertexSize_);

	VBInitialize(dx_->GetDev(), sizeVB, vertexSize_);

	mat_.Initialize();
}

void Model::Initialize(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_)
{
	dx_ = dx_;
	pipeline_ = pipeline_;
	Initialize(shader, filename);
}

Model::Model()
{
}

Model::Model(MyDirectX* dx_, Shader shader, const char* filename, GPipeline* pipeline_)
{
	dx_ = dx_;
	pipeline_ = pipeline_;
	Initialize(shader,filename);
}

void Model::MatUpdate(Matrix matView, Matrix matProjection)
{
	mat_.Update();

	constMapTransform_->mat_ = mat_.matWorld_;
	constMapTransform_->mat_ *= matView;
	constMapTransform_->mat_ *= matProjection;
}

void Model::Draw(size_t handle)
{
	pipeline_->Setting(dx_->GetCmdList());
	pipeline_->Update(dx_->GetCmdList(), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VertBuffUpdate(dx_->GetCmdList());
	//	�e�N�X�`��
	dx_->GetCmdList()->SetGraphicsRootDescriptorTable(1, dx_->GetTextureHandle(handle));
	dx_->GetCmdList()->SetGraphicsRootConstantBufferView(2, transform_->GetGPUVirtualAddress());

	dx_->GetCmdList()->DrawInstanced(vertexSize_, 1, 0, 0);
}

void Model::SetVertices()
{
	//	GPU�������̒l���������悤
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	HRESULT newresult = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(newresult));
	// �S���_�ɑ΂���
	for (size_t i = 0; i < (signed)vertexSize_; i++) {
		vertMap[i] = vertices_[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices_[0]);
}
