#include "MyGame.h"

void MyGame::Initalize()
{
	//
	FlameWork::Initilize();


	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Update()
{
	//基底クラスの更新処理
	FlameWork::Update();

	//SceneChangeAnimation::GetInstance()->Update();

	//シーンマネージャーの更新処理
	SceneManager::GetInstance()->Update();
}

void MyGame::Draw()
{
	SceneManager::GetInstance()->Draw();
}

void MyGame::Finalize()
{
	sceneManager->GetInstance()->Finalize();

	FlameWork::Finalize();
}
