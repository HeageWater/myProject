#pragma once
#include "BaseScene.h"

//���̃V�[���ɂ̂ݒǉ��������
#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Goal.h"
#include "Collision.h"
#include "HitStop.h"
#include "Shake.h"
#include "Emitter.h"
#include "ChengeScene.h"
#include "Warp.h"

class PlayScene : public BaseScene
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