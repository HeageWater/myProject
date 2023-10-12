#include "MyGame.h"
//#include "ChengeScene.h"

void MyGame::Initialize()
{
	//
	FlameWork::Initialize();

	//
	//ChengeScene::GetInstance()->Initialize();
	
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
	SceneManager::GetInstance()->Finalize();

	FlameWork::Finalize();
}
