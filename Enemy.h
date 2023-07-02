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

class Enemy {

public:
	Enemy();
	~Enemy();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(int tex);
	//void Update(Matrix matView, Matrix matProjection, Input* input);
	void Update(Matrix matView, Matrix matProjection);
	bool BoxCollision(Model model);

	bool isDead = false;
	bool deadVec = false;
	Vector3D Vec = {0,0,0};
	void DeadVec();
	float Time = 0;

	void SetTrans(Vector3D trans) { enemy_.mat.trans = trans; };

	Model enemy_;
};