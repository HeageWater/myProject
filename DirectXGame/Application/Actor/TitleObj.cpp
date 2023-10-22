#include "TitleObj.h"
#include "Easing.h"

TitleObj::TitleObj()
{
	time_ = 0;
}

TitleObj::~TitleObj()
{
}

void TitleObj::Initialize(Shader shader, GPipeline* pipeline_)
{
	titleObj_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\Title\\blade.obj", pipeline_);

	titleObj_.mat.Initialize();
	titleObj_.mat.scale = { 5,5,5 };
	titleObj_.mat.trans = { 30,20,0 };
	titleObj_.mat.rotAngle = { 2,0,0 };
}

void TitleObj::Draw(size_t tex)
{
	titleObj_.Draw(tex);
}

void TitleObj::Update(Matrix matView, Matrix matProjection)
{
	if (IsMovie_)
	{
		Movie();
	}

	/*float spd = 0.01f;

	titleObj.mat.rotAngle.z += spd;*/
	titleObj_.MatUpdate(matView, matProjection);
}

bool TitleObj::BoxCollision(Model model)
{
	Vector3D obj = titleObj_.mat.trans;
	float scale = titleObj_.mat.scale.x;

	float a = (model.mat.trans.x - obj.x) * (model.mat.trans.x - obj.x);
	float b = (model.mat.trans.y - obj.y) * (model.mat.trans.y - obj.y);

	float c = model.mat.scale.x * scale;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;
}

void TitleObj::Reset()
{
	titleObj_.mat.Initialize();
	titleObj_.mat.scale = { 5,5,3 };
	titleObj_.mat.trans = { 1000,15,0 };
}

void TitleObj::Movie()
{
	if (!IsMovie_)
	{
		IsMovie_ = true;
	}

	if (IsMovie_)
	{
		if (time_ < 100)
		{
			float maxTime = 15;

			time_++;

			titleObj_.mat.trans.x = (float)Easing::EaseInOut(titleObj_.mat.trans.x, 0, time_ / 100, maxTime);
			titleObj_.mat.trans.y = (float)Easing::EaseInOut(titleObj_.mat.trans.y, 50, time_ / 100, maxTime);

			titleObj_.mat.rotAngle.x = (float)Easing::EaseInOut(titleObj_.mat.trans.x, 0, time_ / 100, maxTime);
			//titleObj.mat.rotAngle.y = (float)Easing::EaseInOut(titleObj.mat.trans.y, 0, time / 100, maxTime);

			titleObj_.mat.scale.x = (float)Easing::EaseInOut(titleObj_.mat.scale.x, 20, time_ / 100, maxTime);
			titleObj_.mat.scale.y = (float)Easing::EaseInOut(titleObj_.mat.scale.y, 20, time_ / 100, maxTime);
		}
		else if (time_ < 150)
		{
			time_++;
		}
		else if (!EndMovie_)
		{
			EndMovie_ = true;
		}
	}
}
