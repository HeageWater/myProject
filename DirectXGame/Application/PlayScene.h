#pragma once
#include "BaseScene.h"

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