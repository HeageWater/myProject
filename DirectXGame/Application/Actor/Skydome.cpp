#include "Skydome.h"

Skydome::Skydome()
{
	//modelを制作
	model_ = std::make_unique<Model>();

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };

	time_ = 0;
}

Skydome::~Skydome()
{
	Destroy();
}

void Skydome::Initialize(Shader shader, GPipeline* pipeline)
{
	pipeline_ = pipeline;

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources\\skydome\\skydome.png");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("Resources\\skydome\\skydome.obj");

	//モデルセット
	model_ = SetModel("Resources\\skydome\\skydome.obj");
}

void Skydome::Draw()
{
	model_->Draw(tex_);
}

Vector2D Skydome::MoveCamera(Matrix matView, Matrix matProjection)
{
	Vector3D move = { 0 ,0 ,0 };

	model_->mat_.trans_ += move;

	model_->MatUpdate(matView, matProjection);

	return Vector2D(move.x_, move.z_);
}

void Skydome::Update(Matrix matView, Matrix matProjection)
{
	matView_ = matView;
	matProjection_ = matProjection;

	//座標Update
	model_->MatUpdate(matView, matProjection);
}
