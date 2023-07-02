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

class Goal
{
public:
	Goal();
	~Goal();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(int tex);
	void Update(Matrix matView, Matrix matProjection);

	bool BoxCollision(Model model);

	Model goal_;
};
