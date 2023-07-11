#include "ImguiManager.h"
//#include <imgui_impl_win32.h>
//#include<imgui_impl_dx12.h>

ImguiManager::ImguiManager()
{
}

ImguiManager::~ImguiManager()
{
}

void ImguiManager::Initialize(MyDirectX* dxCommon)
{
	////ImGuiのコンテキストを生成
	//ImGui::CreateContext();

	////ImGuiのスタイルを生成
	//ImGui::StyleColorsDark();

	////win32用初期化
	//ImGui_ImplWin32_Init(dxCommon->GetWindow().GetHwnd());

	////デスクリプタヒープ設定(クラス化)
	//D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	//desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//desc.NumDescriptors = 1;
	//desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	////デスクリプタヒープ生成(クラス化)
	//HRESULT result;
	//result = dxCommon->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	//assert(SUCCEEDED(result));


	//ImGui_ImplDX12_Init(
	//	dxCommon->GetDev(),
	//	static_cast<int>(dxCommon->GetBackByfferCount()),
	//	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	//	srvHeap_.Get(),
	//	srvHeap_->GetCPUDescriptorHandleForHeapStart(),
	//	srvHeap_->GetGPUDescriptorHandleForHeapStart()
	//);

	//ImGuiIO& io = ImGui::GetIO();
	////標準フォントを追加する
	//io.Fonts->AddFontDefault();
}

void ImguiManager::Finalize()
{
	/*ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	srvHeap_.Reset();*/
}

void ImguiManager::Begin()
{
	//ImGuiフレーム開始
	/*ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();*/
}

void ImguiManager::End()
{//描画前準備(Update寄り)
	//ImGui::Render();
}

void ImguiManager::Draw(MyDirectX* dxCommon)
{
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	////デスクリプタヒープの配列をセットするコマンド
	//ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps),ppHeaps);
	////描画コマンド実行
	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),cmdList);
}