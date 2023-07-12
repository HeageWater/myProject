#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Shader
{
private:
	HRESULT result;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3DBlob> vsBlob;		// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> hsBlob;		// �n���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> dsBlob;		// �h���C���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob;		// �W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;		// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;		// �G���[�I�u�W�F�N�g

private:
	void Error();
public:
	Shader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main", LPCWSTR GSFileName = nullptr, LPCWSTR DSFileName = nullptr, LPCWSTR HSFileName = nullptr);

	//	Getter
	ID3DBlob* VSBlob() { return vsBlob.Get(); }
	ID3DBlob* HSBlob() { return hsBlob.Get(); }
	ID3DBlob* DSBlob() { return dsBlob.Get(); }
	ID3DBlob* GSBlob() { return gsBlob.Get(); }
	ID3DBlob* PSBlob() { return psBlob.Get(); }
};
