#include "ScissorRect.h"

ScissorRect::ScissorRect()
{
}

void ScissorRect::Init(const size_t left, const size_t width, const size_t top, const size_t height)
{
	scissorRect.left = left; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + width; // �؂蔲�����W�E
	scissorRect.top = top; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + height; // �؂蔲�����W��
}

void ScissorRect::Update(ID3D12GraphicsCommandList* cmdList)
{
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	cmdList->RSSetScissorRects(1, &scissorRect);
}
