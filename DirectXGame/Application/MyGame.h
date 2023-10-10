#pragma once
#include "FlameWork.h"
#include "SceneManager.h"
//#include "PostEffect.h"

class MyGame :public FlameWork
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;
private:
	//シーンファクトリー
	std::unique_ptr<SceneFactory>sceneManager;
};