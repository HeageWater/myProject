#pragma once
#include "BaseScene.h"

#include "Player.h"
#include "Stage.h"
#include "Enemy.h"
#include "Goal.h"
#include "Collision.h"
#include "HitStop.h"
#include "Shake.h"
#include "Emitter.h"
#include "ChengeScene.h"

class PlayScene : public BaseScene
{
public:
	void Update() override;
	void Initilize()override;
	void Draw()	   override;
	void Finalize()  override;

private:
	//~PlayScene()override;
};