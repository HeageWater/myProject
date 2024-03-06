#include "Stage.h"
#include "CollisionManager.h"

Stage::Stage()
{
	//modelを制作
	model_ = std::make_unique<Model>();

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };
}

Stage::~Stage()
{
	Destroy();
}

void Stage::Initialize(Shader shader, GPipeline* pipeline_)
{
	model_->Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\box.obj", pipeline_);

	//tag_ = "stage";
	//CollisionManager::GetInstance()->AddCollision(this);

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 5,5,5 };
	model_->mat_.trans_.x_ = 0;
	model_->mat_.trans_.y_ = 0;
	model_->mat_.trans_.z_ = 0;

	drawFlag_ = true;
}

void Stage::Draw(size_t tex)
{
	if (drawFlag_)
	{
		model_->Draw(tex);
	}
}

void Stage::Update(Matrix matView, Matrix matProjection)
{
	model_->MatUpdate(matView, matProjection);
}

void Stage::Reset()
{
	model_->mat_.trans_.x_ = 0;
	model_->mat_.trans_.y_ = 0;
}