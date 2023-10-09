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

class TitleObj
{
public:
	TitleObj();
	~TitleObj();
	void Initialize(Shader shader, GPipeline* pipeline_);
	void Draw(size_t tex);
	void Update(Matrix matView, Matrix matProjection);

	bool BoxCollision(Model model);
	void Reset();
	void Movie();
	bool IsMovie = false;
	bool EndMovie = false;

	Model titleObj;

private:
	float time = 0;
};
