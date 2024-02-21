#include "ImguiManager.h"
#include <imgui_impl_win32.h>
#include<imgui_impl_dx12.h>
#include "DirectX.h"
#include "Window.h"

void ImguiManager::Initialize(Window* window)
{
	//ImGuiのコンテキストを生成
	ImGui::CreateContext();

	//ImGuiのスタイルを生成
	ImGui::StyleColorsDark();

	//win32用初期化
	ImGui_ImplWin32_Init(window->GetHwnd());// dxCommon->GetWindow().GetHwnd());

	//デスクリプタヒープ設定(クラス化)
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	//ここが違いそう
	//デスクリプタヒープ生成(クラス化)
	HRESULT result;
	result = MyDirectX::GetInstance()->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	//この下のsrvHeapのところをシングルトンデスクリプタヒープにしないとImGuiを全部で使えない
	ImGui_ImplDX12_Init(
		MyDirectX::GetInstance()->GetDev(),
		static_cast<int>(MyDirectX::GetInstance()->GetBackByfferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	); 

	//DescriptorHeap::DescriptorHeapViewHandle handle = MyDirectX::GetInstance()->GetDescriptorHeap()->AddSRV();

	////この下のsrvHeapのところをシングルトンデスクリプタヒープにしないとImGuiを全部で使えない
	//ImGui_ImplDX12_Init(
	//	MyDirectX::GetInstance()->GetDev(),
	//	static_cast<int>(MyDirectX::GetInstance()->GetBackByfferCount()),
	//	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	//	MyDirectX::GetInstance()->GetDescriptorHeap()->GetHeap().Get(),
	//	handle.cpuHandle,
	//	handle.gpuHandle
	//);

	ImGuiIO& io = ImGui::GetIO();
	//標準フォントを追加する
	io.Fonts->AddFontDefault();
}

void ImguiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	srvHeap_.Reset();
}

void ImguiManager::Begin()
{
	//ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::End()
{
	//描画前準備(Update寄り)
	ImGui::Render();
}

void ImguiManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = MyDirectX::GetInstance()->GetCmdList();

	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//描画コマンド実行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

ImguiManager* ImguiManager::GetInstance()
{
	static ImguiManager imgui;
	return &imgui;
}
