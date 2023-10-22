#pragma once
#include <d3d12.h>
#include "MyMath.h"
#include <wrl.h>

/// <summary>
/// コンストバッファ
/// </summary>
class ConstBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	//結果
	HRESULT result_;

	//ヒーププロップ
	D3D12_HEAP_PROPERTIES heapProp_{};

	//リソースデスク
	D3D12_RESOURCE_DESC resourceDesc_{};

	//コンストバッファデータマテリアル
	struct ConstBufferDataMaterial 
	{
		//RGBA
		Vector4D color_;
	};

	//コンストバッファデータマテリアル
	struct ConstBufferTimeMaterial 
	{
		float time_;
	};

	//マテリアル
	ComPtr<ID3D12Resource> material_;

	//マップ用マテリアル
	ConstBufferDataMaterial* mapMaterial_ = nullptr;

	//タイムマテリアル
	ComPtr<ID3D12Resource> timeMaterial_;

	//マップ用タイムマテリアル
	ConstBufferTimeMaterial* timeMapMaterial_ = nullptr;

	//タイマー
	float timer_ = 100000;

public:

	ConstBuff(ID3D12Device* dev);
	~ConstBuff();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dev"></param>
	void Initialize(ID3D12Device* dev);

	/// <summary>
	/// 色変更
	/// </summary>
	/// <param name="color"></param>
	void ChangeColor(const Vector4D& color);

	/// <summary>
	/// 時間更新
	/// </summary>
	void TimeUpdate();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cmdList"></param>
	void Update(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// セットバッファビュー
	/// </summary>
	/// <param name="cmdList"></param>
	void SetBufferView(ID3D12GraphicsCommandList* cmdList);
};

