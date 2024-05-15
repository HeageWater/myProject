#pragma once
#include "Bullet.h"
#include "BaseEnemy.h"

class BulletEnemy : public BaseEnemy
{
public://継承しているもの

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(Shader shader, GPipeline* pipeline)override;

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

	/// <summary>
	/// 箱同士の判定(collisionManager作成後削除)
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	bool BoxCollision(MyMath::ObjMatrix model);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="time"></param>
	void SetTime(size_t time) { time_ = time; };

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetDeadVec() { return deadVec_; };

	/// <summary>
	/// 
	/// </summary>
	void SertchPlayer(MyMath::ObjMatrix model);

	/// <summary>
	/// pos入手
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return  model_->mat_.trans_; };

public:

	/// <summary>
	/// 
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
	/// 
	/// </summary>
	void BulletUpdate(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 
	/// </summary>
	std::vector<Bullet*> GetBullet();

	/// <summary>
	/// 攻撃
	/// </summary>
	void EnemyAttack();

	/// <summary>
	/// 
	/// </summary>
	void CreateBullet();

	/// <summary>
	/// モデル情報を確保
	/// </summary>
	/// <returns></returns>
	//Model* GetModel() { return model_; };
private:
	//弾格納用
	std::vector<Bullet*> bullets_;

	//撃つフラグ
	bool fireFlag_;

	float attackTime_ = 0;

	Shader shader_;
	GPipeline* pipeline_;

	//
	Vector3D playerVec = { 0,0,0 };
};