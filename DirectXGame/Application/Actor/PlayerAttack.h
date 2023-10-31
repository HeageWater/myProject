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
#include "Sound.h"
#include "DirectX.h"
#include "GPipeline.h"
#include "Shader.h"

/// <summary>
/// 攻撃生成用モデル
/// </summary>
class PlayerAttack
{
public:
	PlayerAttack(MyDirectX* dx, Shader shader, GPipeline* pipeline);
	~PlayerAttack();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dx_"></param>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(MyDirectX* dx, Shader shader, GPipeline* pipeline);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	void Update(Matrix matView, Matrix matProjection);

	/// <summary>
	/// 
	/// </summary>
	void SetUpdate();

	/// <summary>
	/// posセット
	/// </summary>
	/// <param name="pos_"></param>
	void SetPos(Vector3D pos) { playerAttack_.mat_.trans_ = pos; };

	/// <summary>
	/// 向きセット
	/// </summary>
	/// <param name="vec_"></param>
	void SetVec(Vector2D vec) { this->vec_ = vec; };

	/// <summary>
	/// 死んでいるか
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; };

	/// <summary>
	/// pos取得
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return playerAttack_.mat_.trans_; };

	/// <summary>
	/// scale取得
	/// </summary>
	/// <returns></returns>
	Vector3D GetScale() { return playerAttack_.mat_.scale_; };

	/// <summary>
	///rot取得
	/// </summary>
	/// <returns></returns>
	Vector3D GetRot() { return playerAttack_.mat_.rotAngle_; };

private:

	//サウンド
	MyXAudio* sound_ = nullptr;

	//画像
	size_t tex_ = 0;

	//攻撃用モデル
	Model playerAttack_;

	//攻撃しているかフラグ
	bool attackF_ = false;

	//消えかかる
	bool deleteFlag = false;

	//向き
	Vector2D vec_;

	//どのくらいたったか
	uint32_t time_;

	//死んでいるか
	bool isDead_ = false;

	//コントローラー
	Controller* controller_ = nullptr;
};