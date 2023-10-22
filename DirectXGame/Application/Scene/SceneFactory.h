#pragma once
#include<memory>
#include<string>
#include"BaseScene.h"

/// <summary>
///	主にシーンを生成する
/// </summary>
class SceneFactory
{
public:

	virtual ~SceneFactory() = default;

	//シーン生成
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};

//このゲーム用のシーン工場
class SceneCreate :public SceneFactory
{
public:

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	/// <returns>生成したシーン</returns>
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName)override;
};