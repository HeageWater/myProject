#pragma once
#include "BaseScene.h"

class TitleScene : public BaseScene
{
public:
	void Update() override;
	void Initilize()override;
	void Draw()	   override;
	void Finalize()  override;

private:
	//~TitleScene()override;
};