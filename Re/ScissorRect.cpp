#include "ScissorRect.h"

ScissorRect::ScissorRect()
{
}

void ScissorRect::Init(const float left, const float width, const float top, const float height)
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
