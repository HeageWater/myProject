#include "Bullet.h"
#include "CollisionManager.h"
#include "ParticleManager.h"

Bullet::Bullet()
{

}

void Bullet::Initialize(Shader shader, GPipeline* pipeline)
{
	//modelを制作
	model_ = std::make_unique<Model>();

	//タグ
	tag_ = "bullet";
	CollisionManager::GetInstance()->AddCollision(this);

	//更新関係代入
	shader_ = shader;
	pipeline_ = pipeline;

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("Resources\\Model\\kyu\\kyu.obj");

	//モデルセット
	model_ = SetModel("Resources\\Model\\kyu\\kyu.obj");

	//読み込み
	//model_->Initialize(MyDirectX::GetInstance(), shader_, "Resources\\Model\\kyu\\kyu.obj", pipeline_);

	//初期化
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 5,5,5 };

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");

	//死んでいるかのフラグをoffに
	isDead_ = false;
}

void Bullet::Update()
{
	//進む速度
	const float SPEED = 0.2f;

	//移動
	model_->mat_.trans_ -= vec_ * SPEED;

	//Cameraなど更新
	model_->SetCamera(view_, prodaction_);
	model_->Update();

	//消える瞬間
	Delete();
}

void Bullet::Draw()
{
	//描画
	model_->Draw(tex_);
}

void Bullet::SetCamera(const Matrix& view, const Matrix& prodaction)
{
	//更新関係代入
	view_ = view;
	prodaction_ = prodaction;
}

void Bullet::Delete()
{
	//消える瞬間
	if (GetDeleteFlag())
	{
		isDead_ = true;
	}
}

void Bullet::OnCollision()
{
	ParticleManager::GetInstance()->CreateBoxParticle(model_->mat_.trans_);
}