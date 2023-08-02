#pragma once
#include "BaseScene.h"

class TitleScene : BaseScene
{
public:
	void Update()override;
	void Initilize()override;
	void Draw()override;
	void Finalize()override;

private:
	TitleScene();
	~TitleScene();
};