#include "Bullet.h"

Bullet::Bullet()
{

}

void Bullet::Initialize(Shader shader, GPipeline* pipeline)
{
	//更新関係代入
	shader_ = shader;
	pipeline_ = pipeline;

	//読み込み
	bullet_.Initialize(MyDirectX::GetInstance(), shader_, "Resources\\Model\\kyu\\kyu.obj", pipeline_);

	//初期化
	bullet_.mat_.Initialize();
	bullet_.mat_.scale_ = { 5,5,5 };

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");

	//死んでいるかのフラグをoffに
	isDead_ = false;
}

void Bullet::Update()
{
	//進む速度
	const float SPEED = 0.05f;

	//移動
	bullet_.mat_.trans_ -= Vec * SPEED;

	//Cameraなど更新
	bullet_.SetCamera(view_, prodaction_);
	bullet_.Update();
}

void Bullet::Draw()
{
	//描画
	bullet_.Draw(tex_);
}

void Bullet::SetCamera(Matrix view, Matrix prodaction)
{
	//更新関係代入
	view_ = view;
	prodaction_ = prodaction;
}
