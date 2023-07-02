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

class Player {

public:
	Player();
	~Player();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(int tex, int tex2);
	void Update(Matrix matView, Matrix matProjection, Input* input);
	void Update(Matrix matView, Matrix matProjection);
	void Reset();

	void Jump();
	void Attack();

	//左スティックの値を返す
	Vector2D GetController() { return controller->GetLeftStickVec(); };

	Model player_;
	Model playerAttack_;

	bool attackF = false;

	float jumpPower = 0;
	float gravirtPower = 0;

	Controller* controller = nullptr;
};
