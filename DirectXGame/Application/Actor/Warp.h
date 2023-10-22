#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include <memory>
#include "Model.h"

/// <summary>
/// プレイヤーがワープするときの動き
/// </summary>
class Warp
{
public:
	Warp();
	~Warp();

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
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 四角と四角の判定(collisionManagerに移行後削除)
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	bool BoxCollision(Model model);

	/// <summary>
	/// 描画フラグをセット
	/// </summary>
	/// <param name="flag"></param>
	void SetFlag(bool flag) { drawFlag_ = flag; };

	/// <summary>
	/// 入った場所
	/// </summary>
	/// <returns></returns>
	Vector3D GetInPos() { return warp_[0].mat_.trans_; };

	/// <summary>
	/// 出る場所
	/// </summary>
	/// <returns></returns>
	Vector3D GetOutPos() { return warp_[1].mat_.trans_; };

	/// <summary>
	/// 今どのフェーズか
	/// </summary>
	/// <param name="mode"></param>
	void CheckMode(uint32_t mode);

	//入口と出口用モデル
	Model warp_[2];
private:

	//入口と出口の数
	float size_ = 2;

	//描画するかどうか
	bool drawFlag_ = true;
};