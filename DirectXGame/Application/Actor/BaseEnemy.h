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

class BaseEnemy : public GameModel
{
public:
	virtual ~BaseEnemy() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	virtual void Initialize(Shader shader, GPipeline* pipeline_) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	virtual void Draw() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	virtual void Update(Matrix matView, Matrix matProjection) = 0;

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	//virtual bool GetIsDead() = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="model"></param>
	/// <returns></returns>
	virtual bool BoxCollision(MyMath::ObjMatrix model) = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="time"></param>
	virtual void SetTime(size_t time) = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	virtual bool GetDeadVec() = 0;

	/// <summary>
	/// 
	/// </summary>
	virtual void SertchPlayer(MyMath::ObjMatrix model) = 0;

	/// <summary>
	/// pos入手
	/// </summary>
	/// <returns></returns>
	virtual Vector3D GetPos() = 0;

public:

	//画像
	size_t tex_;

	//死んでいるか
	bool isDead_ = false;

	//動く量
	Vector3D move_ = { 0,0,0 };

	//どのくらいたったか
	size_t time_ = 0;

	//死んだときに吹っ飛ぶ
	bool deadVec_ = false;

	uint16_t hitpoint = 1;
};