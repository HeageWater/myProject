#pragma once
#include "BaseScene.h"

class TitleScene :public BaseScene
{
public:
	//������
	void Initilize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�j��
	void Finalize()override;
};