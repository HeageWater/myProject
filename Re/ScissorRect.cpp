#include "ScissorRect.h"

ScissorRect::ScissorRect()
{
}

void ScissorRect::Init(const int left, const int width, const int top, const int height)
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
