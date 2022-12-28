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
#include "Re//Controller.h"


class Player {

public:
	Player();
	~Player();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(int tex);
	void Update(Matrix matView, Matrix matProjection);

	void Jump();
	void Attack();

	Model player;
	Model attackModel;
	Controller* controller = nullptr;

	bool jumpFlag;
	float jumpPower;
	float GravityPower;

	Vector2D attackRange;
};
