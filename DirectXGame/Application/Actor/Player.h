#pragma once
#include "PlayerAttack.h"
#include "ImGuiManager.h"

class Player {

public:
	Player();
	~Player();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex, size_t tex2);
	Vector2D MoveCamera(Matrix matView, Matrix matProjection, Input* input);
	void Update(Matrix matView, Matrix matProjection, MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Reset();
	void Jump();
	void Attack(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);

	//攻撃生成用
	void PopPlayerAttack();

	//左スティックの値を返す
	Vector2D GetController() { return controller->GetLeftStickVec(); };

	Vector3D GetPos() { return player_.mat.trans; };
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
	bool StageCollsion(Model stage, Matrix matView, Matrix matProjection);
	bool GetA()
	{ //コントローラーUpdate
		controller->Update(); 
		return controller->ButtonTriggerPush(A);
	};
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

	float Life;
};
