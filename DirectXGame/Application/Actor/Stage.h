#pragma once
#include "GPipeline.h"
#include "Object3D.h"
#include "ConstBuff.h"
#include "Shader.h"
#include "MyDebugCamera.h"
#include "Square.h"
#include <memory>
#include "Model.h"

class Stage
{
public:
	Stage();
	~Stage();
	void Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex);
	void Update(Matrix matView, Matrix matProjection);
	void Reset();

	Model stage_;
private:
};