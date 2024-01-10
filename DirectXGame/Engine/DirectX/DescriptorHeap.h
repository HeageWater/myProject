#pragma once
#include<wrl.h>
#include <d3d12.h>
#include<stdint.h>

/// <summary>
/// デスクプリタヒープ
/// </summary>
class DescriptorHeap
{
private:

	//デバイス
	Microsoft::WRL::ComPtr <ID3D12Device> device_;
	//デスクプリタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap_;
	//デスクプリタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange_{};

	char PADING_[4];

	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE	startGpuHandle_;

	uint32_t incrementSize_ = 0;

	size_t maxSRV_ = 200000;
	size_t maxUAV_ = 2048;
	size_t maxCBV_ = 200000;

	size_t countSRV_ = 0;
	size_t countUAV_ = 0;
	size_t countCBV_ = 0;

public:

	struct DescriptorHeapViewHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// シェーダーリソースビュー生成
	/// </summary>
	/// <param name="desc">シェーダーリソースビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// アンオーダーアクセスビュー生成
	/// </summary>
	/// <param name="desc">アンオーダーアクセスビューデスク</param>
	/// <param name="resource">バッファ</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateUAV(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// コンストバッファビュー生成
	/// </summary>
	/// <param name="desc">コンストバッファビューデスク</param>
	/// <returns>GPUハンドル(UINT64)</returns>
	UINT64 CreateCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc);

	DescriptorHeapViewHandle AddSRV();

	/// <summary>
	/// デスクプリタヒープを取得
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetHeap();
};