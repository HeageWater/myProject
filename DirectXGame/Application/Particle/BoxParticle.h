#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "Model.h"
#include "Square.h"

/// <summary>
/// 箱型パーティクル
/// </summary>
class BoxParticle
{
public:
	BoxParticle();
	~BoxParticle();

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
	void Draw(size_t tex);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void Update(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; };

	/// <summary>
	/// 場所セット
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3D pos) { box_.mat_.trans_ = pos; };

	/// <summary>
	/// 向きセット
	/// </summary>
	/// <param name="vec"></param>
	void SetVec(Vector3D vec) { velocity_ = vec; };

	/// <summary>
	/// 場所ゲット
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return box_.mat_.trans_; };

	/// <summary>
	/// 向きゲット
	/// </summary>
	/// <returns></returns>
	Vector3D GetVec() { return velocity_; };
private:

	//Square box;
	Object3D box_;

	//死んでいるか
	bool isDead_ = false;

	//速さ
	float spd_;

	//どれくらいたったか
	float time_ = 0;

	//向き
	Vector3D velocity_ = { 0,0,0 };
};