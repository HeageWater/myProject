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

/// <summary>
/// ゴール作成クラス
/// </summary>
class Goal
{
public:
	Goal();
	~Goal();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dx_"></param>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);

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
	bool BoxCollision(MyMath::ObjMatrix model);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// posをセット
	/// </summary>
	void SetPos(Vector3D pos);

	//ゴール用モデル
	Model goal_;
};
