#pragma once
#include "FlameWork.h"
#include "SceneManager.h"
//#include "PostEffect.h"

/// <summary>
/// ゲームループの根幹
/// </summary>
class MyGame :public FlameWork
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

private:
	//シーンファクトリー
	std::unique_ptr<SceneFactory>sceneManager_;
};