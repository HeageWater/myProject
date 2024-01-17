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

	/// <summary>
	/// 箱同士の判定(collisionManager作成後削除)
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	bool BoxCollision(Model model);

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
	void SertchPlayer(Model model);

	/// <summary>
	/// pos入手
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return  enemy_.mat_.trans_; };

public:

	/// <summary>
	/// 
	/// </summary>
	void DeadVec();

	/// <summary>
	/// transセット
	/// </summary>
	/// <param name="trans"></param>
	void SetTrans(Vector3D trans) { enemy_.mat_.trans_ = trans; };

	/// <summary>
	/// scaleセット
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3D scale) { enemy_.mat_.scale_ = scale; };

	/// <summary>
	/// rotationセット
	/// </summary>
	/// <param name="scale"></param>
	void SetRot(Vector3D rotAngle) { enemy_.mat_.rotAngle_ = rotAngle; };

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

private:
	//弾格納用
	std::vector<Bullet*> bullets_;

	//撃つフラグ
	bool fireFlag_;

	size_t attackTime_ = 0;

	Shader shader_;
	GPipeline* pipeline_;

	//
	Vector3D playerVec = { 0,0,0 };
};