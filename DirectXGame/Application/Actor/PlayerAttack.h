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

class PlayerAttack
{
public:
	PlayerAttack(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	~PlayerAttack();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw();
	void Update(Matrix matView, Matrix matProjection);

	void SetUpdate();
	void SetPos(Vector3D pos_) { playerAttack_.mat.trans = pos_; };
	void SetVec(Vector2D vec_) { this->vec = vec_; };
	bool GetIsDead() { return isDead; };

	Vector3D GetPos() { return playerAttack_.mat.trans; };
	Vector3D GetScale() { return playerAttack_.mat.scale; };
	Vector3D GetRot() { return playerAttack_.mat.rotAngle; };
private:
	MyXAudio* sound_ = nullptr;
	//size_t volcano = 0;
	size_t tex = 0;
	Model playerAttack_;

	bool attackF = false;
	Vector2D vec;
	uint32_t time;
	bool isDead = false;

	Controller* controller = nullptr;
};