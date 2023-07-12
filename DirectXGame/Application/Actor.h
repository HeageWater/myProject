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

class Actor {

protected:
	static void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	static void Draw(size_t tex);
	static void Update(Matrix matView, Matrix matProjection);

	Model actor;
};
