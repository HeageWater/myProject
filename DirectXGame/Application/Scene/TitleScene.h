#pragma once
#include "BaseScene.h"

class TitleScene :public BaseScene
{
public:
	//‰Šú‰»
	void Initilize()override;

	//XV
	void Update()override;

	//•`‰æ
	void Draw()override;

	//”jŠü
	void Finalize()override;
};