#include "Bullet.h"

Bullet::Bullet()
{

}

void Bullet::Initialize(Shader shader, GPipeline* pipeline)
{
	shader_ = shader;
	pipeline_ = pipeline;

	bullet_.Initialize(MyDirectX::GetInstance(), shader_, "Resources\\Model\\kyu\\kyu.obj", pipeline_);

	bullet_.mat_.Initialize();
	bullet_.mat_.scale_ = { 1,1,1 };

	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/Model/ene/enemy.png");
}

void Bullet::Update()
{
	const float SPEED = 0.1f;

	//
	bullet_.mat_.trans_ -= Vec * SPEED;

	//
	bullet_.SetCamera(view_, prodaction_);
	bullet_.Update();
}

void Bullet::Draw()
{
	bullet_.Draw(tex_);
}

void Bullet::SetCamera(Matrix view, Matrix prodaction)
{
	view_ = view;
	prodaction_ = prodaction;
}
