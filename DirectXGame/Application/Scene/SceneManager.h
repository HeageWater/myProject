#pragma once
#include"BaseScene.h"
#include"SceneFactory.h"

class SceneManager
{
private:
	std::unique_ptr<BaseScene> scene_;
	std::unique_ptr<BaseScene> nextScene_;

	//シーンファクトリー
	SceneFactory* sceneFactory_ = nullptr;

	//シーンの名前
	std::string sceneName_;
	std::string nextSceneName_;

public:
	//更新
	void Update();

	//描画
	void Draw();

	//終了
	void Finalize();

	//次シーン
	void ChangeScene(const std::string& sceneName);

	//
	void SetSceneFactory(SceneFactory* sceneFactory);

	//
	std::string GetSceneName();

	//シングルトン
	static SceneManager* GetInstance();

private:
	SceneManager() = default;
	~SceneManager() = default;

	//コピーコンストラクタ・代入演算子削除
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(const SceneManager&) = delete;
};