#include "Shader.h"
#include <string>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

Shader::Shader()
{
	result = {};
}

Shader::Shader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint, LPCWSTR GSFileName, LPCWSTR DSFileName, LPCWSTR HSFileName)
{
	result = S_OK;

#pragma region VertexShader
	//	頂点シェーダファイル読み込み＆コンパイル
	result = D3DCompileFromFile(
		VSFileName,									// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
		pEntryPoint, "vs_5_0",									// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
		0,
		&vsBlob_, &errorBlob_);

	// エラーなら
	Error();
#pragma endregion

#pragma region HS
	if (HSFileName != nullptr) {
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			HSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "hs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&hsBlob_, &errorBlob_);

		// エラーなら
		Error();
	}
#pragma endregion

#pragma region DS
	if (DSFileName != nullptr) {
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			DSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "ds_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&dsBlob_, &errorBlob_);

		// エラーなら
		Error();
	}
#pragma endregion

#pragma region GS
	if (GSFileName != nullptr) {
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			GSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "gs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&gsBlob_, &errorBlob_);

		// エラーなら
		Error();
	}
#pragma endregion

#pragma region PixelShader
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		PSFileName, // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		pEntryPoint, "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob_, &errorBlob_);

	// エラーなら
	Error();
#pragma endregion
}

void Shader::Initizlize(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint, LPCWSTR GSFileName, LPCWSTR DSFileName, LPCWSTR HSFileName)
{
	result = S_OK;

#pragma region VertexShader
	//	頂点シェーダファイル読み込み＆コンパイル
	result = D3DCompileFromFile(
		VSFileName,									// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
		pEntryPoint, "vs_5_0",									// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
		0,
		&vsBlob_, &errorBlob_);

	// エラーなら
	Error();
#pragma endregion

#pragma region HS
	if (HSFileName != nullptr) {
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			HSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "hs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&hsBlob_, &errorBlob_);

		// エラーなら
		Error();
	}
#pragma endregion

#pragma region DS
	if (DSFileName != nullptr) {
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			DSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "ds_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&dsBlob_, &errorBlob_);

		// エラーなら
		Error();
	}
#pragma endregion

#pragma region GS
	if (GSFileName != nullptr) {
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			GSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "gs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&gsBlob_, &errorBlob_);

		// エラーなら
		Error();
	}
#pragma endregion

#pragma region PixelShader
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		PSFileName, // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		pEntryPoint, "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob_, &errorBlob_);

	// エラーなら
	Error();
#pragma endregion
}

void Shader::Error()
{
	result = S_OK;

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}
