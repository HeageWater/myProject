#include"SceneFactory.h"
#include"TitleScene.h"
#include"PlayScene.h"
#include"GameScene.h"

//
std::unique_ptr<BaseScene> SceneCreate::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "PLAY")
	{
		//newScene = new GameScene();
		newScene = new PlayScene();
	}
	else if (sceneName == "")
	{

	}

	return std::unique_ptr<BaseScene>(newScene);
}
