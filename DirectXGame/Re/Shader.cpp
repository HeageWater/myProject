#include "Shader.h"
#include <string>
#include <cassert>

Shader::Shader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint,LPCWSTR GSFileName, LPCWSTR DSFileName, LPCWSTR HSFileName)
{
#pragma region VertexShader
	//	���_�V�F�[�_�t�@�C���ǂݍ��݁��R���p�C��
	result = D3DCompileFromFile(
		VSFileName,									// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					// �C���N���[�h�\�ɂ���
		pEntryPoint, "vs_5_0",									// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	Error();
#pragma endregion
	
#pragma region HS
	if (HSFileName != nullptr) {
		//	���_�V�F�[�_�t�@�C���ǂݍ��݁��R���p�C��
		result = D3DCompileFromFile(
			HSFileName,									// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// �C���N���[�h�\�ɂ���
			pEntryPoint, "hs_5_0",									// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// �f�o�b�O�p�ݒ�
			0,
			&hsBlob, &errorBlob);

		// �G���[�Ȃ�
		Error();
	}
#pragma endregion

#pragma region DS
	if (DSFileName != nullptr) {
		//	���_�V�F�[�_�t�@�C���ǂݍ��݁��R���p�C��
		result = D3DCompileFromFile(
			DSFileName,									// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// �C���N���[�h�\�ɂ���
			pEntryPoint, "ds_5_0",									// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// �f�o�b�O�p�ݒ�
			0,
			&dsBlob, &errorBlob);

		// �G���[�Ȃ�
		Error();
	}
#pragma endregion

#pragma region GS
	if (GSFileName != nullptr) {
		//	���_�V�F�[�_�t�@�C���ǂݍ��݁��R���p�C��
		result = D3DCompileFromFile(
			GSFileName,									// �V�F�[�_�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// �C���N���[�h�\�ɂ���
			pEntryPoint, "gs_5_0",									// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// �f�o�b�O�p�ݒ�
			0,
			&gsBlob, &errorBlob);

		// �G���[�Ȃ�
		Error();
	}
#pragma endregion

#pragma region PixelShader
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		PSFileName, // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		pEntryPoint, "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	Error();
#pragma endregion
}

void Shader::Error()
{
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}
