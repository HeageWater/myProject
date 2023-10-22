#pragma once
#include"BaseScene.h"
#include"SceneFactory.h"

/// <summary>
/// シーンをまとめる処理
/// </summary>
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

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 次のシーンへ
	/// </summary>
	/// <param name="sceneName">次のシーンの名前</param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// シーンファクトリーにセット
	/// </summary>
	/// <param name="sceneFactory"></param>
	void SetSceneFactory(SceneFactory* sceneFactory);

	/// <summary>
	/// 今のシーンの名前を返す
	/// </summary>
	/// <returns></returns>
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