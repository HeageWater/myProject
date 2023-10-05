#pragma once
#include<memory>
#include<string>
#include"BaseScene.h"

class SceneFactory
{
public:

	virtual ~SceneFactory() = default;

	//�V�[������
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};