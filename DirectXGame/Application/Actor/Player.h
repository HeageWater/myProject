#pragma once
#include "PlayerAttack.h"
#include "ImGuiManager.h"

class Player {

public:
	Player();
	~Player();
	void Initialize(Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex, size_t tex2);
	Vector2D MoveCamera(Matrix matView, Matrix matProjection, Input* input);
	void Update(Matrix matView, Matrix matProjection, Shader shader);
	void Reset();
	void Jump();
	void Attack(Shader shader);

	//攻撃生成用
	void PopPlayerAttack();

	//左スティックの値を返す
	Vector2D GetController() { return controller->GetLeftStickVec(); };

	Vector3D GetPos() { return player_.mat.trans; };
	void SetPos(Vector3D pos) { player_.mat.trans = pos; };
	Vector3D GetRot() { return player_.mat.rotAngle; };
	Vector3D GetScale() { return player_.mat.scale; };
	Model GetModel() { return player_; };

	Vector3D GetAttackPos() { return playerAttack_.mat.trans; };
	Vector3D GetAttackRot() { return playerAttack_.mat.rotAngle; };
	Vector3D GetAttackScale() { return playerAttack_.mat.scale; };
	Model GetAttackModel() { return playerAttack_; };

	bool CollisionAttackToEnemy(Model enemy);
	//bool CollisionPlayerToEnemy(Model enemy);

	float GetLife() { return Life; };
	void SetLife(float x) { Life = x; };
	bool StageCollsion(Model stage, Matrix matView, Matrix matProjection);
	bool StageCollsionX(Model stage);
	bool StageCollsionY(Model stage);
	bool GetA()
	{ //コントローラーUpdate
		controller->Update();
		return controller->ButtonTriggerPush(A);
	};

	bool StageCollision(Model stage);

	bool PlayerCollision(Model enemy);

	//ワープするアクション
	bool WarpAction();

	//ワープの入口に触れた時
	bool warpActionFlag = false;

	uint32_t warpMord = 0;

	//warpの入口と出口のpos
	Vector3D warpPos[2];

	void SetWarpPos(Vector3D pos1, Vector3D pos2) { warpPos[0] = pos1; warpPos[1] = pos2; };

	uint32_t GetWarpMode() { return warpMord; };
	void SetWarpMode(uint32_t mode) { warpMord = mode; };

	void MoveX() { player_.mat.trans.x += colVec.x; };
	void MoveY() { player_.mat.trans.y += colVec.y; };

private:
	MyXAudio* sound_ = nullptr;
	//size_t volcanoSE = 0;
	size_t jumpSE = 0;

	Model player_;
	Model playerAttack_;

	std::vector<PlayerAttack*> attack;

	bool attackF = false;
	bool createAttackFlag = false;

	float jumpPower = 0;
	float gravirtPower = 0;

	Controller* controller = nullptr;

	//移動用変数
	Vector3D colVec = { 0,0,0 };

	//HP
	float Life;

	//点滅カウント
	uint32_t lesFlag;

	//ノックバックの向き
	float knockBackVec = 0;

	//ノックバックのフラグ
	bool knockBackFlag;

	//敵と当たった時にHP減らしてノックバックのフラグをONに
	//点滅フラグもONに
	void LesLife();

	//敵と当たった時のノックバック
	void KnockBack();
};
