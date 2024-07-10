#pragma once
#include "GameModel.h"

/// <summary>
/// 天球
/// </summary>
class Skydome :GameModel
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skydome();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(Shader shader, GPipeline* pipeline_);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	/// <param name="tex2"></param>
	void Draw();

	/// <summary>
	/// プレイヤー基準でカメラ動かす
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="input"></param>
	/// <returns></returns>
	Vector2D MoveCamera(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="shader"></param>
	void Update(Matrix matView, Matrix matProjection);

private:

	//
	GPipeline* pipeline_;

	//
	size_t tex_;

	//
	Matrix matView_;
	Matrix matProjection_;

	//タイム
	size_t time_;

};