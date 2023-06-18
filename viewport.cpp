#include "viewport.h"

//������(�Œ�l)
Port::Port()
{
	//�r���[�ݒ�
	viewport.Width = 1280;
	viewport.Height = 720;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//�V�U�[�ݒ�
	scissorRec.left = 0;				 //�؂蔲�����W��
	scissorRec.right = 1280;	 //�؂蔲�����W�E
	scissorRec.top = 0;				 //�؂蔲�����W��
	scissorRec.bottom = 720;	 //�؂蔲�����W��
}

//������(���)
Port::Port(size_t Width, size_t Height)
{
	//�r���[�ݒ�
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//�V�U�[�ݒ�
	scissorRec.left = 0;				 //�؂蔲�����W��
	scissorRec.right = Width;	 //�؂蔲�����W�E
	scissorRec.top = 0;				 //�؂蔲�����W��
	scissorRec.bottom = Height;	 //�؂蔲�����W��
}

//�`��͈͂̕ύX
void Port::ChengePort(size_t Width, size_t Height)
{
	viewport.Width = Width;
	viewport.Height = Height;
}

//�|�[�g�͈�
void Port::DrawViewPort(ID3D12GraphicsCommandList* commandList)
{
	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetViewports(1, &viewport);
}

//�V�U�[�͈�
void Port::DrawScissor(ID3D12GraphicsCommandList* commandList)
{
	//�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetScissorRects(1, &scissorRec);
}

