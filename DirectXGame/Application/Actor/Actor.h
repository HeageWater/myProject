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

class Actor {

protected:
	static void Initialize(Shader shader, GPipeline* pipeline_);
	static void Draw();
	static void Update(Matrix matView, Matrix matProjection);
	// virtual
	static void SetTag(std::string tag);

	Model* model_;
	size_t tex_;
	std::string tag_;
};
