#include "Warp.h"

Warp::Warp()
{
	for (size_t i = 0; i < size; i++)
	{
		warp_[i].mat.Initialize();
		warp_[i].mat.scale = { 3,3,3 };
	}
}

Warp::~Warp()
{
}

void Warp::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	for (size_t i = 0; i < size; i++)
	{
		warp_[i].Initialize(dx_, shader, "Resources\\Model\\kyu\\kyu.obj", pipeline_);

		warp_[i].mat.Initialize();
		warp_[i].mat.scale = { 5,5,5 };
		warp_[0].mat.trans = { 280,-160,0 };
		warp_[1].mat.trans = { 350,-160,0 };
	}
}

void Warp::Draw(size_t tex)
{
	for (size_t i = 0; i < size; i++)
	{
		//warp_.mat.rotAngle.y += 0.01f;

		warp_[i].Draw(tex);
	}
}

void Warp::Update(Matrix matView, Matrix matProjection)
{
	float spd = 0.01f;

	for (size_t i = 0; i < size; i++)
	{
		warp_[i].mat.rotAngle.y += spd;
		warp_[i].MatUpdate(matView, matProjection);
	}
}

bool Warp::BoxCollision(Model model)
{
	float a = (model.mat.trans.x - warp_[0].mat.trans.x) * (model.mat.trans.x - warp_[0].mat.trans.x);
	float b = (model.mat.trans.y - warp_[0].mat.trans.y) * (model.mat.trans.y - warp_[0].mat.trans.y);

	float c = model.mat.scale.x * warp_[0].mat.scale.x;

	//‚ ‚½‚è”»’è
	if (a + b < c)
	{
		return true;
	}

	return false;
}

void Warp::CheckMode(uint32_t mode)
{
	float spd = 0.3f;

	if (mode == 2)
	{
		warp_[0].mat.scale -= {spd, spd, spd};
		if (warp_[0].mat.scale.x < 0)
		{
			warp_[0].mat.scale = { 0,0,0 };
		}
	}
	else if (mode == 3)
	{
		warp_[1].mat.scale -= {spd, spd, spd};
		if (warp_[1].mat.scale.x < 0)
		{
			warp_[1].mat.scale = { 0,0,0 };
		}
	}
}

void Warp::Reset()
{
	for (size_t i = 0; i < size; i++)
	{
		warp_[i].mat.Initialize();
		warp_[i].mat.scale = { 5,5,3 };
		warp_[i].mat.trans = { 1000,15,0 };
	}
}