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

class Enemy {

public:
	Enemy();
	~Enemy();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex);
	//void Update(Matrix matView, Matrix matProjection, Input* input);
	void Update(Matrix matView, Matrix matProjection);
	bool BoxCollision(Model model);
	void SertchPlayer(Model model);

	bool isDead = false;
	bool deadVec = false;
	Vector3D Vec = {0,0,0};
	Vector3D move = {0,0,0};
	void DeadVec();
	float Time = 0;

	void SetTrans(Vector3D trans) { enemy_.mat.trans = trans; };
	void SetScale(Vector3D scale) { enemy_.mat.scale = scale; };

	Vector3D GetPos() {return  enemy_.mat.trans;};

	Model enemy_;
	bool sertchFlag = false;
};