#pragma once
#include "FlameWork.h"
#include "SceneManager.h"
//#include "PostEffect.h"

class MyGame :public FlameWork
{
public:
	void Initalize();
	void Update();
	void Draw();
	void Finalize();
private:
	//シーンファクトリー
	std::unique_ptr<SceneManager>sceneManager;
};