#include "TitleObj.h"
#include "Easing.h"

TitleObj::TitleObj()
{
	time = 0;
}

TitleObj::~TitleObj()
{
}

void TitleObj::Initialize(Shader shader, GPipeline* pipeline_)
{
	titleObj.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\Title\\blade.obj", pipeline_);

	titleObj.mat.Initialize();
	titleObj.mat.scale = { 5,5,5 };
	titleObj.mat.trans = { 30,20,0 };
	titleObj.mat.rotAngle = { 2,0,0 };
}

void TitleObj::Draw(size_t tex)
{
	titleObj.Draw(tex);
}

void TitleObj::Update(Matrix matView, Matrix matProjection)
{
	if (IsMovie)
	{
		Movie();
	}

	/*float spd = 0.01f;

	titleObj.mat.rotAngle.z += spd;*/
	titleObj.MatUpdate(matView, matProjection);
}

bool TitleObj::BoxCollision(Model model)
{
	Vector3D obj = titleObj.mat.trans;
	float scale = titleObj.mat.scale.x;

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
	titleObj.mat.Initialize();
	titleObj.mat.scale = { 5,5,3 };
	titleObj.mat.trans = { 1000,15,0 };
}

void TitleObj::Movie()
{
	if (!IsMovie)
	{
		IsMovie = true;
	}

	if (IsMovie)
	{
		if (time < 100)
		{
			float maxTime = 15;

			time++;

			titleObj.mat.trans.x = (float)Easing::EaseInOut(titleObj.mat.trans.x, 0, time / 100, maxTime);
			titleObj.mat.trans.y = (float)Easing::EaseInOut(titleObj.mat.trans.y, 50, time / 100, maxTime);

			titleObj.mat.rotAngle.x = (float)Easing::EaseInOut(titleObj.mat.trans.x, 0, time / 100, maxTime);
			//titleObj.mat.rotAngle.y = (float)Easing::EaseInOut(titleObj.mat.trans.y, 0, time / 100, maxTime);

			titleObj.mat.scale.x = (float)Easing::EaseInOut(titleObj.mat.scale.x, 20, time / 100, maxTime);
			titleObj.mat.scale.y = (float)Easing::EaseInOut(titleObj.mat.scale.y, 20, time / 100, maxTime);
		}
		else if (time < 150)
		{
			time++;
		}
		else if (!EndMovie)
		{
			EndMovie = true;
		}
	}
}
