#pragma once
#include "BaseEnemy.h"

class BossEnemy : BaseEnemy
{
public:
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
	const bool GetIsDead() override;

private:

	void Move();

	void Attack();

	void GetLife();

private:
	Shader shader_;
	GPipeline* pipeline_;

};