#include "Post.h"
#include <d3dx12.h>

using namespace DirectX;

Post::Post():Sprite()
{
	//sprite
}

void Post::Draw(ID3D12GraphicsCommandList* cmdList)
{

	//cmdList->SetPipelineState(pipelineStateP.Get());

	//cmdList->SetGraphicsRootSignature(rootSignatureP.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//頂点バッファビューの設定コマンド
	spriteCommon_->dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	//デスクリプタ
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1,);

	//画像を1に入れたものに
	//spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, handle);

	//定数バッファビュー(CBV)の設定コマンド
	spriteCommon_->dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//描画コマンド
	//spriteCommon_->dxCommon_->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
	spriteCommon_->dxCommon_->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}
