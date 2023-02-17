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
	bool lerpF = 0;
	bool hitF = 0;
	bool deathF = 0;
	float spd = 1.0f;

	float startCount = 0;
	float nowCount = 0;
	float elapsedCount = 0;

	//補完で使うデータ
	Vector3D p0 = { -10.0f, 0.0f, 0.0f };
	Vector3D p1 = { 35.0f, 10.0f, 5.0f };
	Vector3D p2 = { 65.0f, 20.0f, 5.0f };
	Vector3D p3 = { 75.0f, 40.0f, 0.0f };

	Vector3D a = { -100.0f, 0.0f, 0.0f };
	Vector3D b = { -100.0f, 0.0f, 0.0f };

	//時間
	float maxTime = 0.5f;
	float timeRate;

	//球の位置
	Vector3D position;
};
