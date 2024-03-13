#include "MyGame.h"
#include "ChengeScene.h"

void MyGame::Initialize()
{
	//フレームワーク初期化
	FlameWork::Initialize();

	//共通カメラ


	//シーンチェンジ初期化
	//ChengeScene::GetInstance()->Initialize();

	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TUTORIAL");
	//SceneManager::GetInstance()->ChangeScene("PLAY");
}

void MyGame::Update()
{
	//基底クラスの更新処理
	FlameWork::Update();

	//シーンチェンジ更新
	//ChengeScene::GetInstance()->Update();

	//シーンマネージャーの更新処理
	SceneManager::GetInstance()->Update();
}

void MyGame::Draw()
{
	//シーンマネージャーの描画処理
	SceneManager::GetInstance()->Draw();

	//SceneChenge描画
	//ChengeScene::GetInstance()->Draw();
}

void MyGame::Finalize()
{
	//シーンマネージャーの終了処理
	SceneManager::GetInstance()->Finalize();

	//フレームワークの終了処理
	FlameWork::Finalize();
}
