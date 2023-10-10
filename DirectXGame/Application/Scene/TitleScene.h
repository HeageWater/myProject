#pragma once
#include "BaseScene.h"

class TitleScene :public BaseScene
{
public:
	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//破棄
	void Finalize()override;
};