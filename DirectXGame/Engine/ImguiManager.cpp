#include "ImguiManager.h"
#include <imgui_impl_win32.h>
#include<imgui_impl_dx12.h>

ImguiManager::ImguiManager()
{
}

ImguiManager::~ImguiManager()
{
}

void ImguiManager::Initialize(MyDirectX* dxCommon)
{
	//ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();

	//ImGui�̃X�^�C���𐶐�
	ImGui::StyleColorsDark();

	//win32�p������
	ImGui_ImplWin32_Init(dxCommon->GetWindow().GetHwnd());

	//�f�X�N���v�^�q�[�v�ݒ�(�N���X��)
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//�f�X�N���v�^�q�[�v����(�N���X��)
	HRESULT result;
	result = dxCommon->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));


	ImGui_ImplDX12_Init(
		dxCommon->GetDev(),
		static_cast<int>(dxCommon->GetBackByfferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvHeap_.Get(),
		srvHeap_->GetCPUDescriptorHandleForHeapStart(),
		srvHeap_->GetGPUDescriptorHandleForHeapStart()
	);

	ImGuiIO& io = ImGui::GetIO();
	//�W���t�H���g��ǉ�����
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
	//ImGui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::End()
{
	//�`��O����(Update���)
	ImGui::Render();
}

void ImguiManager::Draw(MyDirectX* dxCommon)
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�`��R�}���h���s
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

//ImguiManager* ImguiManager::GetInstance()
//{
//	static ImguiManager imgui;
//	return &imgui;
//}
