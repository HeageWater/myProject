#include "SceneFactory.h"
#include"TitleScene.h"
#include"PlayScene.h"
#include"GameScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	//Ÿ‚ÌƒV[ƒ“‚ğ¶¬
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "GAME")
	{
		//newScene = new GameScene();
		newScene = new PlayScene();
	}

	return std::unique_ptr<BaseScene>(newScene);
}