#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include <memory>
#include "Model.h"
#include "GameModel.h"

class Bullet :GameModel
{
public:
	/// <summary>
	/// 
	/// </summary>
	Bullet();

	/// <summary>
	/// 
	/// </summary>
	void Initialize(Shader shader, GPipeline* pipeline);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	void Draw();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="view"></param>
	/// <param name="prodaction"></param>
	void SetCamera(const Matrix& view, const Matrix& prodaction);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Vector3D& pos) { model_->mat_.trans_ = pos; };

	/// <summary>
	/// 
	/// </summary>
	/// <param name="vec"></param>
	void SetVec(const Vector3D& vec) { vec_ = vec; };

	/// <summary>
	/// 
	/// </summary>
	const bool GetIsDead() { return isDead_; };

	/// <summary>
	/// 消滅
	/// </summary>
	void Delete();

	/// <summary>
	/// どこかに当たった時
	/// </summary>
	void OnCollision();
private:

	//画像
	size_t tex_ = 0;

	//死んでいるかのフラグ
	bool isDead_ = false;

	//更新関係
	Shader shader_;
	GPipeline* pipeline_;
	Matrix view_;
	Matrix prodaction_;
};