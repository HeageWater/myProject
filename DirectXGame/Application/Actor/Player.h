#pragma once
//#include "GPipeline.h"
//#include "Object3D.h"
//#include "ConstBuff.h"
//#include "TextureData.h"
//#include "Shader.h"
//#include "MyDebugCamera.h"
//#include "Square.h"
//#include "Controller.h"
//#include <memory>
//#include "Model.h"
//#include "Sound.h"
#include "PlayerAttack.h"

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

	//�U�������p
	void PopPlayerAttack();

	//���X�e�B�b�N�̒l��Ԃ�
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
private:
	MyXAudio* sound_ = nullptr;
	size_t volcano = 0;

	Model player_;
	Model playerAttack_;

	std::vector<PlayerAttack*> attack;

	bool attackF = false;
	bool createAttackFlag = false;

	float jumpPower = 0;
	float gravirtPower = 0;

	Controller* controller = nullptr;
};
