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

	titleObj_.mat_.Initialize();
	titleObj_.mat_.scale_ = { 5,5,5 };
	titleObj_.mat_.trans_ = { 30,20,0 };
	titleObj_.mat_.rotAngle_ = { 2,0,0 };
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
	Vector3D obj = titleObj_.mat_.trans_;
	float scale = titleObj_.mat_.scale_.x_;

	float a = (model.mat_.trans_.x_ - obj.x_) * (model.mat_.trans_.x_ - obj.x_);
	float b = (model.mat_.trans_.y_ - obj.y_) * (model.mat_.trans_.y_ - obj.y_);

	float c = model.mat_.scale_.x_ * scale;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;
}

void TitleObj::Reset()
{
	titleObj_.mat_.Initialize();
	titleObj_.mat_.scale_ = { 5,5,3 };
	titleObj_.mat_.trans_ = { 1000,15,0 };
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

			titleObj_.mat_.trans_.x_ = (float)Easing::EaseInOut(titleObj_.mat_.trans_.x_, 0, time_ / 100, maxTime);
			titleObj_.mat_.trans_.y_ = (float)Easing::EaseInOut(titleObj_.mat_.trans_.y_, 50, time_ / 100, maxTime);

			titleObj_.mat_.rotAngle_.x_ = (float)Easing::EaseInOut(titleObj_.mat_.trans_.x_, 0, time_ / 100, maxTime);
			//titleObj.mat.rotAngle.y = (float)Easing::EaseInOut(titleObj.mat.trans.y, 0, time / 100, maxTime);

			titleObj_.mat_.scale_.x_ = (float)Easing::EaseInOut(titleObj_.mat_.scale_.x_, 20, time_ / 100, maxTime);
			titleObj_.mat_.scale_.y_ = (float)Easing::EaseInOut(titleObj_.mat_.scale_.y_, 20, time_ / 100, maxTime);
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
