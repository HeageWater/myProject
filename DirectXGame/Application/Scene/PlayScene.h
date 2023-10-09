#pragma once
#include "BaseScene.h"

//このシーンにのみ追加するもの
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
#include "TitleObj.h"

class PlayScene : public BaseScene
{
public:
	//初期化
	void Initilize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//破棄
	void Finalize()override;
};