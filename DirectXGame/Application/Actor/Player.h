#pragma once
#include "PlayerAttack.h"
#include "ImGuiManager.h"
#include "GameModel.h"

/// <summary>
/// ゲーム中の自機
/// </summary>
class Player :GameModel
{
	//関数
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="shader"></param>
	/// <param name="pipeline_"></param>
	void Initialize(Shader shader, GPipeline* pipeline_);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="tex"></param>
	/// <param name="tex2"></param>
	void Draw(size_t tex);

	/// <summary>
	/// 
	/// </summary>
	void Draw()override;

	/// <summary>
	/// プレイヤー基準でカメラ動かす
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="input"></param>
	/// <returns></returns>
	Vector2D MoveCamera(Matrix matView, Matrix matProjection, Input* input);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <param name="shader"></param>
	void Update(Matrix matView, Matrix matProjection);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 左スティックの値を返す
	/// </summary>
	/// <returns></returns>
	Vector2D GetController() { return controller_->GetLeftStickVec(); };

	/// <summary>
	/// posを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetPos() { return model_->mat_.trans_; };

	/// <summary>
	///posをセットする
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(Vector3D pos) { model_->mat_.trans_ = pos; };

	/// <summary>
	/// scaleをセットする
	/// </summary>
	/// <param name="pos"></param>
	void SetScale(Vector3D scale) { model_->mat_.scale_ = scale; };

	/// <summary>
	/// rotaionを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetRot() { return model_->mat_.rotAngle_; };

	/// <summary>
	/// scaleを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetScale() { return model_->mat_.scale_; };

	/// <summary>
	/// 震える
	/// </summary>
	void Sheik();

	/// <summary>
	/// matview,matProdactionをセット
	/// </summary>
	void SetCamera(Matrix matView, Matrix matProjection);

public:

	/// <summary>
	/// HPがゼロになった時のアニメーション
	/// </summary>
	bool DeadAnimation();

	/// <summary>
	/// 死亡時に一回呼び出す
	/// </summary>
	void SetDeadAnimation();

	/// <summary>
	/// 
	/// </summary>
	void OnCollision()override;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	MyMath::ObjMatrix GetMat() { return model_->mat_; };

private:

	//パイプライン
	GPipeline* pipeline_;

	//音
	MyXAudio* sound_ = nullptr;

	//コントローラー
	Controller* controller_ = nullptr;

	//
	size_t tex_;

	//
	Matrix matView_;
	Matrix matProjection_;

	//タイム
	size_t time_;
};