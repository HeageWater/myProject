#include "Goal.h"

Goal::Goal()
{
	goal_.mat.Initialize();
	goal_.mat.scale = { 3,3,3 };
}

Goal::~Goal()
{
}

void Goal::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	goal_.Initialize(dx_, shader, "Resources\\Model\\ene\\ene.obj", pipeline_);

	goal_.mat.Initialize();
	goal_.mat.scale = { 5,5,3 };
	goal_.mat.trans = { 380,-160,0 };
}

void Goal::Draw(size_t tex)
{
	goal_.mat.rotAngle.y += 0.01f;

	goal_.Draw(tex);
}

void Goal::Update(Matrix matView, Matrix matProjection)
{
	goal_.MatUpdate(matView, matProjection);
}

bool Goal::BoxCollision(Model model)
{
	float a = (model.mat.trans.x - goal_.mat.trans.x) * (model.mat.trans.x - goal_.mat.trans.x);
	float b = (model.mat.trans.y - goal_.mat.trans.y) * (model.mat.trans.y - goal_.mat.trans.y);

	float c = model.mat.scale.x * goal_.mat.scale.x;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;
}

void Goal::Reset()
{
	goal_.mat.Initialize();
	goal_.mat.scale = { 5,5,3 };
	goal_.mat.trans = { 1000,15,0 };
}