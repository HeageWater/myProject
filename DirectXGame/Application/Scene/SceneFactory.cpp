#include"SceneFactory.h"
#include"TitleScene.h"
#include"PlayScene.h"
#include"GameClearScene.h"
#include"GameOverScene.h"
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
		newScene = new PlayScene();
	}
	else if (sceneName == "GAMECLEAR")
	{
		newScene = new GameClearScene();
	}
	else if (sceneName == "GAMEOVER")
	{
		newScene = new GameOverScene();
	}

	return std::unique_ptr<BaseScene>(newScene);
}
