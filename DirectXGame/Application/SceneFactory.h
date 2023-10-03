#pragma once
#include<memory>
#include"BaseScene.h"

#include<string>

/// <summary>
/// �V�[���H��(�T�O)
/// </summary>
class SceneFactory
{
public:

	virtual ~SceneFactory() = default;

	//�V�[������
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};