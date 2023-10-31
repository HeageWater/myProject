#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include <memory>
#include "Model.h"

/// <summary>
/// パーティクル用基盤
/// </summary>
class Particle
{
public:
	Particle();
	~Particle();

	/// <summary>
	/// posをセットすることでその周りにパーティクルがb描画される
	/// </summary>
	/// <param name="pos"></param>
	void SetInitialize(Vector3D pos);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dx_"></param>
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
	/// 消えたか
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; };

	/// <summary>
	/// モデルを返す
	/// </summary>
	/// <returns></returns>
	Model GetModel() { return particle_; };

	/// <summary>
	/// モデルをセットする
	/// </summary>
	/// <param name="model"></param>
	void SetModel(Model model) { particle_ = model; };

	/// <summary>
	/// 生成する場所をセット
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3D pos);

	//今どれくらいたったか
	float time_ = 0;
private:

	//パーティクル一個
	Model particle_;

	//死んでいるか
	bool isDead_ = false;

	//速さ
	float spd_;

	//向き
	Vector3D velocity_ = { 0,0,0 };
};