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
	void Draw(size_t tex, size_t tex2);

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
	void Update(Matrix matView, Matrix matProjection, Shader shader);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 攻撃生成用
	/// </summary>
	void PopPlayerAttack();

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
	/// modelを返す
	/// </summary>
	/// <returns></returns>
	//Model* GetModel() { return model_; };

	/// <summary>
	/// 攻撃のposを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetAttackPos() { return playerAttack_.mat_.trans_; };

	/// <summary>
	/// 攻撃のrotationを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetAttackRot() { return playerAttack_.mat_.rotAngle_; };

	/// <summary>
	/// 攻撃のscaleを返す
	/// </summary>
	/// <returns></returns>
	Vector3D GetAttackScale() { return playerAttack_.mat_.scale_; };

	/// <summary>
	/// 攻撃のmodelを返す
	/// </summary>
	/// <returns></returns>
	Model GetAttackModel() { return playerAttack_; };

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::vector<PlayerAttack*> GetAttack()
	{
		return attack_;
	}

	/// <summary>
	/// 攻撃を敵の判定
	/// </summary>
	/// <param name="enemy"></param>
	/// <returns></returns>
	bool CollisionAttackToEnemy(Model enemy);
	//bool CollisionPlayerToEnemy(Model enemy);

	/// <summary>
	/// HPを返す
	/// </summary>
	/// <returns></returns>
	float GetLife() { return life_; };

	/// <summary>
	/// HPをセットする
	/// </summary>
	/// <param name="x"></param>
	void SetLife(float x) { life_ = x; };

	/// <summary>
	/// ステージとの判定
	/// </summary>
	/// <param name="stage"></param>
	/// <param name="matView"></param>
	/// <param name="matProjection"></param>
	/// <returns></returns>
	bool StageCollsion(MyMath::ObjMatrix stage, Matrix matView, Matrix matProjection);

	/// <summary>
	/// ステージとの横判定
	/// </summary>
	/// <param name="stage"></param>
	/// <returns></returns>
	bool StageCollsionX(MyMath::ObjMatrix stage);

	/// <summary>
	/// ステージとの縦判定
	/// </summary>
	/// <param name="stage"></param>
	/// <returns></returns>
	bool StageCollsionY(MyMath::ObjMatrix stage);

	/// <summary>
	/// ステージとの判定(多分使わなくなるので消すこと)
	/// </summary>
	/// <param name="stage"></param>
	/// <returns></returns>
	bool StageCollision(MyMath::ObjMatrix stage);

	/// <summary>
	/// playerと敵との判定
	/// </summary>
	/// <param name="enemy"></param>
	/// <returns></returns>
	bool PlayerCollision(MyMath::ObjMatrix enemy);

	/// <summary>
	/// ワープするアクション
	/// </summary>
	/// <returns></returns>
	bool WarpAction();

	/// <summary>
	/// ワープするとき外からでもposを変えられるように
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	void SetWarpPos(Vector3D pos1, Vector3D pos2) { warpPos_[0] = pos1; warpPos_[1] = pos2; };

	/// <summary>
	/// 今のワープ行動の種類を入手
	/// </summary>
	/// <returns></returns>
	uint32_t GetWarpMode() { return warpMord_; };

	/// <summary>
	/// ワープの行動の種類をセット
	/// </summary>
	/// <param name="mode"></param>
	void SetWarpMode(uint32_t mode) { warpMord_ = mode; };

	/// <summary>
	/// 横移動のみ
	/// </summary>
	void MoveX() { model_->mat_.trans_.x_ += colVec_.x_; };

	/// <summary>
	/// 縦移動のみ
	/// </summary>
	void MoveY() { model_->mat_.trans_.y_ += colVec_.y_; };

	/// <summary>
	/// 震える
	/// </summary>
	void Sheik();

	/// <summary>
	/// matview,matProdactionをセット
	/// </summary>
	void SetCamera(Matrix matView, Matrix matProjection);
private:

	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 攻撃
	/// </summary>
	/// <param name="shader"></param>
	void Attack(Shader shader);

	/// <summary>
	/// 
	/// </summary>
	void WallRightKick();

	/// <summary>
	/// 
	/// </summary>
	void WallLeftKick();

public:

	/// <summary>
	/// ジャンプ用アニメーション
	/// </summary>
	void JumpAnimation();

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

	//ワープの入口に触れた時
	bool warpActionFlag_ = false;

	//ワープの種類
	uint32_t warpMord_ = 0;

	//warpの入口と出口のpos
	Vector3D warpPos_[2];

private:

	//ワープの種類
	uint8_t deadAnimationMode_ = 0;

	//死亡時いた場所
	Vector3D deadAnimationPos_;

private:

	GPipeline* pipeline_;

	//音
	MyXAudio* sound_ = nullptr;

	//ジャンプ音
	size_t jumpSE_ = 0;

	//モデルデータ
	//Model model_;

	//攻撃用モデルデータ
	Model playerAttack_;

	//攻撃を何個も作成するための変数
	std::vector<PlayerAttack*> attack_;

	//攻撃フラグ
	bool attackF_ = false;

	//攻撃を作成するフラグ
	bool createAttackFlag_ = false;

	//ジャンプ力
	float jumpPower_ = 0;

	//重力
	float gravityPower_ = 0;

	//コントローラー
	Controller* controller_ = nullptr;

	//移動用変数
	Vector3D colVec_ = { 0,0,0 };

	//HP
	float life_;

	//点滅カウント
	uint32_t lesFlag_;

	//ノックバックの向き
	float knockBackVec_ = 0;

	//ノックバックのフラグ
	bool knockBackFlag_;

	//
	size_t sheikF_;

	//
	size_t whiteTex_;

	//
	Matrix matView_;
	Matrix matProjection_;

	//タイム
	size_t time_;

	//壁キックするフラグ
	bool leftKick_ = false;
	bool rightKick_ = false;

	//ジャンプのアニメーションをするためのフラグ
	bool jumpAnimationF_ = false;

	//壁キックの進む方向
	Vector3D kickVec_ = { 0,0,0 };

	//今何回ジャンプしているか
	float jumpCount = 0;

	//敵と当たった時にHP減らしてノックバックのフラグをONに
	//点滅フラグもONに
	void LesLife();

	//敵と当たった時のノックバック
	void KnockBack();

	//ジャンプ時の演出
	std::unique_ptr<Model>donatu_{};
};
