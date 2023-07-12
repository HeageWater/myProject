#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "TextureData.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include "Controller.h"
#include <memory>
#include "Model.h"

class Enemy {

public:
	Enemy();
	~Enemy();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex);
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