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

class Enemy {
public:
	Enemy();
	~Enemy();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(int tex, Matrix matView, Matrix matProjection);
	void Update(Matrix matView, Matrix matProjection);

	Model enemy;
	Vector3D trans;
	Vector3D rot;
	bool hitF = 0;
	float spd = 1.0f;
};