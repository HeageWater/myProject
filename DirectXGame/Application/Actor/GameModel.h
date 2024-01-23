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
	bool deleteFlag_ = false;//これがtrueになっているオブジェクトは削除されたものとして扱うべし。

public:

	//デストラクタ
	virtual ~GameModel() = default;

	//初期化処理
	virtual void Initialize() {};

	//更新処理
	virtual void Update() {};

	//描画処理
	virtual void Draw() {};

	//オブジェクトの名前取得処理
	virtual std::string GetName() { return ""; };

	//オブジェクトの衝突判定処理
	virtual void OnCollision() {};

	//死亡フラグの取得
	virtual const bool GetIsDead() { return false; };

	virtual const bool GetIsDamageObject() { return false; };

	//死亡アニメーションフラグの取得
	virtual const bool GetDeathAnimationFlag() { return false; };

	void Destroy() { deleteFlag_ = true; };

	bool GetDeleteFlag() { return deleteFlag_; };

	bool IsKnockBack() { return knockBackFlame_ > 0; };

	//Vector3D vec
	virtual void SetKnockBack() {};
	virtual bool GetNotKnockBack() { return notKnockBackFlame_ > 0; };
	virtual void NotKnockBackTimer() { };

	//const uint16_t& num
	virtual void Appearance() {};

	virtual void EffectCreate() {};

public:
	//static void SetCamera(Camera* camera) { camera_ = camera; };
	//static Camera* GetCamera() { return camera_; };
};