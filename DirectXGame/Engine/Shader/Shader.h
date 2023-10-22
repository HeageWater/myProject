#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>

/// <summary>
/// シェーダークラス
/// </summary>
class Shader
{
private:
	HRESULT result;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> vsBlob_;

	// ハルシェーダオブジェクト
	ComPtr<ID3DBlob> hsBlob_;

	// ドメインシェーダオブジェクト
	ComPtr<ID3DBlob> dsBlob_;

	// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob_;

	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob_;

	// エラーオブジェクト
	ComPtr<ID3DBlob> errorBlob_;

private:

	/// <summary>
	/// エラーが起きた時の処理
	/// </summary>
	void Error();

public:
	Shader();
	Shader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main", LPCWSTR GSFileName = nullptr, LPCWSTR DSFileName = nullptr, LPCWSTR HSFileName = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="VSFileName"></param>
	/// <param name="PSFileName"></param>
	/// <param name="pEntryPoint"></param>
	/// <param name="GSFileName"></param>
	/// <param name="DSFileName"></param>
	/// <param name="HSFileName"></param>
	void Initizlize(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main", LPCWSTR GSFileName = nullptr, LPCWSTR DSFileName = nullptr, LPCWSTR HSFileName = nullptr);

	//	Getter

	/// <summary>
	/// 頂点シェーダ取得
	/// </summary>
	/// <returns></returns>
	ID3DBlob* VSBlob() { return vsBlob_.Get(); }

	/// <summary>
	/// ハルシェーダ取得
	/// </summary>
	/// <returns></returns>
	ID3DBlob* HSBlob() { return hsBlob_.Get(); }

	/// <summary>
	/// ドメインシェーダシェーダ取得
	/// </summary>
	/// <returns></returns>
	ID3DBlob* DSBlob() { return dsBlob_.Get(); }

	/// <summary>
	/// ジオメトリシェーダ取得
	/// </summary>
	/// <returns></returns>
	ID3DBlob* GSBlob() { return gsBlob_.Get(); }

	/// <summary>
	/// ピクセルシェーダ取得
	/// </summary>
	/// <returns></returns>
	ID3DBlob* PSBlob() { return psBlob_.Get(); }
};
