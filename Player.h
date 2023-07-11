#pragma once
#include "Re//GPipeline.h"
#include "Re//Object3D.h"
#include "Re//ConstBuff.h"
#include "Re//TextureData.h"
#include "Re//Shader.h"
#include "Re//MyDebugCamera.h"
#include "Re//Square.h"
#include "Re//Controller.h"
#include <memory>
#include "Re//Model.h"
#include "PlayerAttack.h"
#include "Sound.h"

class Player {

public:
	Player();
	~Player();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex, size_t tex2);
	Vector2D MoveCamera(Matrix matView, Matrix matProjection, Input* input);
	void Update(Matrix matView, Matrix matProjection);
	void Reset();
	void Jump();
	void Attack();

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
private:
	MyXAudio* sound_ = nullptr;
	size_t volcano = 0;

	Model player_;
	Model playerAttack_;

	bool attackF = false;

	float jumpPower = 0;
	float gravirtPower = 0;

	Controller* controller = nullptr;
};
