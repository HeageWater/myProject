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

class BaseEnemy
{
protected:
	virtual ~BaseEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	//virtual void Initialize(Shader shader, GPipeline* pipeline_) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	//virtual void Draw(size_t tex) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	//virtual void Update(Matrix matView, Matrix matProjection) = 0;

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	//virtual bool GetIsDead() = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	//virtual Model GetModel() = 0;

private:

	//死んでいるか
	bool isDead_ = false;

	//向き
	Vector3D Vec_ = { 0,0,0 };

	//動く量
	Vector3D move_ = { 0,0,0 };

	//モデル
	Model enemy_;
};