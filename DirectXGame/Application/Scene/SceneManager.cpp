#include "SceneManager.h"
#include "CollisionManager.h"
#include<cassert>

void SceneManager::Update()
{
	//次シーンの予約があるなら
	if (nextScene_)
	{
		//旧シーン終了
		if (scene_)
		{
			scene_->Finalize();
			scene_.reset();
		}

		//シーン切り替え
		scene_ = std::move(nextScene_);

		//リセット
		CollisionManager::GetInstance()->Reset();

		//次シーンを初期化する
		scene_->Initialize();

		//シーンの名前を変更
		sceneName_ = nextSceneName_;
	}

	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

void SceneManager::Finalize()
{
	scene_->Finalize();
	scene_.reset();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//次シーンを生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
	nextSceneName_ = sceneName;
}

void SceneManager::SetSceneFactory(SceneFactory* sceneFactory)
{
	sceneFactory_ = sceneFactory;
}

std::string SceneManager::GetSceneName()
{
	return sceneName_;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager manager;
	return &manager;
}