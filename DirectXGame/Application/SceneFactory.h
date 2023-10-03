#pragma once
#include<memory>
#include"BaseScene.h"

#include<string>

/// <summary>
/// シーン工場(概念)
/// </summary>
class SceneFactory
{
public:

	virtual ~SceneFactory() = default;

	//シーン生成
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};