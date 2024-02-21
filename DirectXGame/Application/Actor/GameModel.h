#pragma once
#include"myMath.h"
#include <string>
#include"Model.h"
//#include"Camera.h"
//#include"Transform.h"
//#include"DrawOversight.h"

struct CollisionData
{
	Vector3D center = {};
	float radius = 0.0f;

	Vector3D rayStartPos = {};
	Vector3D rayEndPos = {};
};

//オブジェクト基底クラス
class GameModel
{
private:
	//static Camera* camera_;

public:

	//
	Model model_{};
	//方向ベクトル
	Vector2D directionalVec_{};
	CollisionData collisionData{};
	//std::unique_ptr<Model>model_{};
	uint32_t modelTex_ = 0;
	std::string tag_ = "";
	Vector3D knockBackVec_ = {};
	uint32_t knockBackFlame_ = 0;
	uint32_t notKnockBackFlame_ = 0;

private:

	//これがtrueになっているオブジェクトは削除されたものとして扱う
	bool deleteFlag_ = false;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameModel() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// オブジェクトの名前取得処理
	/// </summary>
	/// <returns></returns>
	virtual std::string GetName() { return tag_; };

	/// <summary>
	/// オブジェクトの衝突判定処理
	/// </summary>
	virtual void OnCollision() {};

	/// <summary>
	/// 死亡フラグの取得
	/// </summary>
	/// <returns></returns>
	virtual const bool GetIsDead() { return false; };

	/// <summary>
	/// 死亡アニメーションフラグの取得
	/// </summary>
	/// <returns></returns>
	virtual const bool GetDeathAnimationFlag() { return false; };

	/// <summary>
	/// 破棄
	/// </summary>
	void Destroy() { deleteFlag_ = true; };

	/// <summary>
	/// 消滅フラグ
	/// </summary>
	/// <returns></returns>
	bool GetDeleteFlag() { return deleteFlag_; };

	/// <summary>
	/// ノックバック
	/// </summary>
	/// <returns></returns>
	bool IsKnockBack() { return knockBackFlame_ > 0; };

	//Vector3D vec
	virtual void SetKnockBack() {};
	virtual bool GetNotKnockBack() { return notKnockBackFlame_ > 0; };
	virtual void NotKnockBackTimer() { };

	//const uint16_t& num
	virtual void Appearance() {};

	/// <summary>
	/// 
	/// </summary>
	virtual void EffectCreate() {};

public:
	//static void SetCamera(Camera* camera) { camera_ = camera; };
	//static Camera* GetCamera() { return camera_; };
};