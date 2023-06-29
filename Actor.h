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

class Actor {

protected:
	static void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	static void Draw(int tex);
	static void Update(Matrix matView, Matrix matProjection);

	Model actor;
};
