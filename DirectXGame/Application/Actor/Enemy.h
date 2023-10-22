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
/// 敵生成用クラス(基盤ではない)
/// </summary>
class Enemy 
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 初期日
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
	void SertchPlayer(Model model);

	/// <summary>
	/// ぶっ飛んでいるか
	/// </summary>
	void DeadVec();

	/// <summary>
	/// transセット
	/// </summary>
	/// <param name="trans"></param>
	void SetTrans(Vector3D trans) { enemy_.mat.trans = trans; };

	/// <summary>
	/// scaleセット
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3D scale) { enemy_.mat.scale = scale; };

	/// <summary>
	/// pos入手
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return  enemy_.mat.trans; };


	//死んでいるか
	bool isDead_ = false;

	//死んだときに吹っ飛ぶ方向
	bool deadVec_ = false;

	//向き
	Vector3D Vec_ = {0,0,0};

	//動く量
	Vector3D move_ = {0,0,0};

	//どのくらいたったか
	float Time_ = 0;

	//モデル
	Model enemy_;

	//索敵範囲にいるか
	bool sertchFlag_ = false;
};