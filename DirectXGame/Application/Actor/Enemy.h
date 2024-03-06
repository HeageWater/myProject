#pragma once
#include "BaseEnemy.h"
#include "GameModel.h"

/// <summary>
/// 敵生成用クラス(基盤ではない)
/// </summary>
class Enemy :GameModel
{
public:
	Enemy();
	~Enemy();

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
	/// 箱同士の判定(collisionManager作成後削除)
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	bool BoxCollision(Model model);

	/// <summary>
	/// プレイヤーが一定範囲に入ったら行動開始(collisionManager作成後削除)
	/// </summary>
	/// <param name="model"></param>
	void SertchPlayer(Model* model);

	/// <summary>
	/// ぶっ飛んでいるか
	/// </summary>
	void DeadVec();

	/// <summary>
	/// transセット
	/// </summary>
	/// <param name="trans"></param>
	void SetTrans(Vector3D trans) { model_->mat_.trans_ = trans; };

	/// <summary>
	/// scaleセット
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3D scale) { model_->mat_.scale_ = scale; };

	/// <summary>
	/// rotationセット
	/// </summary>
	/// <param name="scale"></param>
	void SetRot(Vector3D rotAngle) { model_->mat_.rotAngle_ = rotAngle; };

	/// <summary>
	/// pos入手
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return  model_->mat_.trans_; };

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	//bool GetIsDead() { return isDead_; };

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	//Model* GetModel() { return model_; };

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetDeadVec() { return deadVec_; };

	/// <summary>
	/// 
	/// </summary>
	/// <param name="time"></param>
	void SetTime(size_t time) { Time_ = time; };

private:

	//死んでいるか
	bool isDead_ = false;

	//死んだときに吹っ飛ぶ
	bool deadVec_ = false;

	//動く量
	Vector3D move_ = { 0,0,0 };

	//どのくらいたったか
	size_t Time_ = 0;

	//索敵範囲にいるか
	bool sertchFlag_ = false;
};