#include "PlayerAttack.h"
#include "Easing.h"
#include "CollisionManager.h"

PlayerAttack::PlayerAttack(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	Initialize(dx_, shader, pipeline_);
}

PlayerAttack::~PlayerAttack()
{
}

void PlayerAttack::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	tag_ = "attack";
	CollisionManager::GetInstance()->AddCollision(this);

	//modelを制作
	model_ = std::make_unique<Model>();

	//モデル読み込み
	//ModelManager::GetInstance()->LoadModel("Resources\\Model\\Player\\Player.obj");

	//モデルセット
	//model_ = SetModel("Resources\\Model\\Attack\\attack2.obj");

	model_->Initialize(dx_, shader, "Resources\\Model\\Attack\\attack2.obj", pipeline_);

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 10,2,1 };

	/*model1_.Initialize(dx_, shader, "Resources\\Model\\Attack\\attackFront.obj", pipeline_);

	model1_.mat_.Initialize();
	model1_.mat_.scale_ = { 1,1,1 };
	model1_.mat_.rotAngle_ = { 0,0,3 };*/

	controller_ = Controller::GetInstance();
	attackF_ = false;

	sound_ = MyXAudio::GetInstance();

	time_ = 10;
	isDead_ = false;
	deleteFlag = false;
}

void PlayerAttack::Draw()
{
	model_->Draw(tex_);
	//model1_.Draw(tex_);
}

void PlayerAttack::Update(Matrix matView, Matrix matProjection)
{
	time_--;

	if (time_ <= 1)
	{
		isDead_ = true;
		deleteFlag = true;
		Destroy();
	}

	float maxScale = model_->mat_.scale_.x_ * 1.5f;
	float spd = 10.0f;

	if (!deleteFlag)
	{
		if (model_->mat_.scale_.x_ < maxScale)
		{
			model_->mat_.scale_.x_ += spd;
			model1_.mat_.scale_.x_ += spd;
		}
		else
		{
			deleteFlag = true;
		}

		if (model_->mat_.scale_.y_ < maxScale / 4)
		{
			model_->mat_.scale_.y_ += spd;
			model1_.mat_.scale_.y_ += spd;
		}
	}

	if (deleteFlag)
	{
		if (model_->mat_.scale_.x_ > 0)
		{
			model_->mat_.scale_.x_ -= spd;
			model1_.mat_.scale_.x_ -= spd;
		}

		if (model_->mat_.scale_.y_ > 0)
		{
			model_->mat_.scale_.y_ -= spd;
			model1_.mat_.scale_.y_ -= spd;
		}
	}

	//先端
	/*model1_.mat_.trans_ = model_->mat_.trans_;
	model1_.mat_.trans_.x_ += model_->mat_.scale_.x_;
	model1_.mat_.scale_ = model_->mat_.scale_;*/

	model_->MatUpdate(matView, matProjection);
	//model1_.MatUpdate(matView, matProjection);
}

void PlayerAttack::SetUpdate()
{
	float angle = 3.1415f / 2;

	if (vec_.x_ > 0)
	{
		angle *= 2;
	}

	vec_.normalize();
	model_->mat_.rotAngle_.z_ = (angle * vec_.y_);
	//model1_.mat_.rotAngle_.z_ = (angle * vec_.y_);
}
