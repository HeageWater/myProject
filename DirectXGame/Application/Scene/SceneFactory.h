#pragma once
#include<memory>
#include<string>
#include"BaseScene.h"

class SceneFactory
{
public:

	virtual ~SceneFactory() = default;

	//シーン生成
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};