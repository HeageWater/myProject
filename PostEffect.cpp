#include "PostEffect.h"
#include "WindowApi.h"
#include <d3dx12.h>

using namespace DirectX;

PostEffect::PostEffect():Sprite()
{
}

PostEffect::~PostEffect()
{
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//spriteCopy

	//���[���h���W�̍X�V
	//�萔�o�b�t�@�Ƀf�[�^�]��

	//cmdList->SetPipelineState(pipelineState.Get());

	//cmdList->SetGraphicsRootSignature(roorSingature.Get

	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e��R�}���h���X�g
}

void PostEffect::Initialize()
{
	//HRESULT result;

	//Sprite::Inilialize();

	/*CD3DX12_RESOURCE_DESC texresDesx = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WindowApi::window_width,
		(UINT)WindowApi::window_height,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);*/

	//result = device

}
