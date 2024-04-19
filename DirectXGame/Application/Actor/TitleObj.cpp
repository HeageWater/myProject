#include "TitleObj.h"
#include "Easing.h"
#include <cmath>

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
	titleObj_.mat_.trans_ = { 30,-10,0 };
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

bool TitleObj::BoxCollision(Model* model)
{
	Vector3D obj = titleObj_.mat_.trans_;
	float scale = titleObj_.mat_.scale_.x_ * 3;

	float a = (model->mat_.trans_.x_ - obj.x_) * (model->mat_.trans_.x_ - obj.x_);
	float b = (model->mat_.trans_.y_ - obj.y_) * (model->mat_.trans_.y_ - obj.y_);

	float c = model->mat_.scale_.x_ * scale;

	//あたり判定
	if (a + b < c)
	{
		return true;
	}

	return false;

	//// OBBの方向ベクトルeb2を分離軸と設定（すでに標準化されているとします）
	//Vector3D L = titleObj_.mat_.trans_ - model->mat_.trans_;

	//// OBB Aの軸を取得（軸の長さに定数倍します）
	//Vector3D ea1 = model->mat_.scale_;
	//Vector3D ea2 = titleObj_.mat_.scale_;

	//// rAおよびrBを算出
	//FLOAT ra = fabs((L.dot(ea1))) + fabs((L.dot(ea2)));
	//FLOAT rb = fabs((L.dot(ea1))) + fabs((L.dot(ea2)));

	//// 中心点間の距離を算出
	//FLOAT Interval = fabs(((L.dot(titleObj_.mat_.trans_ - model->mat_.trans_))));

	//// 衝突判定
	//if (Interval < ra + rb)
	//	return true;   // 衝突

	//return false;   // 衝突していない
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
			//最大時間
			float maxTime = 15;

			//インクリメント
			time_++;

			//制御時間をまとめた
			float nowTime = time_ / 100;

			//pos,scale,rotの変数
			//最終的Pos
			float endPosX = 0;
			float endPosY = 20;

			//最終的Rot
			float endRotX = 0;
			//float endRotY = 0;

			//最終的Scale
			float endScaleX = 20;
			float endScaleY = 20;

			titleObj_.mat_.trans_.x_ = (float)Easing::EaseInOut(titleObj_.mat_.trans_.x_, endPosX, nowTime, maxTime);
			titleObj_.mat_.trans_.y_ = (float)Easing::EaseInOut(titleObj_.mat_.trans_.y_, endPosY, nowTime, maxTime);

			titleObj_.mat_.rotAngle_.x_ = (float)Easing::EaseInOut(titleObj_.mat_.trans_.x_, endRotX, nowTime, maxTime);
			//titleObj.mat.rotAngle.y = (float)Easing::EaseInOut(titleObj.mat.trans.y, endRotY, nowTime, maxTime);

			titleObj_.mat_.scale_.x_ = (float)Easing::EaseInOut(titleObj_.mat_.scale_.x_, endScaleX, nowTime, maxTime);
			titleObj_.mat_.scale_.y_ = (float)Easing::EaseInOut(titleObj_.mat_.scale_.y_, endScaleY, nowTime, maxTime);
		}
		else if (time_ < 130)
		{
			time_++;
		}
		else if (!EndMovie_)
		{
			EndMovie_ = true;
		}
	}
}
