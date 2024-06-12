#pragma once
#include"myMath.h"
#include <string>
#include"Model.h"
#include"ModelManager.h"
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

	//メインモデル
	std::unique_ptr<Model>model_{};
	//Model* modelTest_;

	//画像
	uint32_t modelTex_ = 0;

	//コリジョン用のタグ
	std::string tag_ = "";

	//向き
	Vector3D vec_;

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
	/// 位置を返す
	/// </summary>
	virtual Vector3D GetPos() { return model_->mat_.trans_; };

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

	//Vector3D vec
	virtual void SetKnockBack() {};

	//向きを決める
	virtual void SetVec(Vector3D vec) { vec_ = vec; };

	//const uint16_t& num
	virtual void Appearance() {};

	/// <summary>
	/// 
	/// </summary>
	virtual void EffectCreate() {};

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	MyMath::ObjMatrix GetMat() { return model_->mat_; };

	/// <summary>
	/// モデル読み込み
	/// </summary>
	void LoadModel(const std::string& filename)
	{
		ModelManager::GetInstance()->LoadModel(filename);
	};

	/// <summary>
	/// モデルをセットする
	/// </summary>
	/// <returns></returns>
	std::unique_ptr<Model> SetModel(const std::string& filename)
	{
		return ModelManager::GetInstance()->FindModel(filename);
	};

public:
	//static void SetCamera(Camera* camera) { camera_ = camera; };
	//static Camera* GetCamera() { return camera_; };
};