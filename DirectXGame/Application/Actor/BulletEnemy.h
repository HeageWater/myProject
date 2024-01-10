#pragma once
#include "BaseEnemy.h"

class BulletEnemy : BaseEnemy
{

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(Shader shader, GPipeline* pipeline_)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	void Draw()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void Update(Matrix matView, Matrix matProjection)override;

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() override;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Model GetModel() override;

};