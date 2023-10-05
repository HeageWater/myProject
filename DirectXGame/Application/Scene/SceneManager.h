#pragma once
#include "BaseScene.h"
#include"SceneFactory.h"

class SceneManager
{
public:
	std::unique_ptr<BaseScene> scene_;

private:

	void Update();
	void Initialize();
	void Draw();
	void Finalize();
};