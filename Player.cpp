#include "Player.h"
#include "Easing.h"
#include "CollisionManager.h"
#include "ModelManager.h"
#include <imgui.h>

Player::Player()
{
	//modelを制作
	model_ = std::make_unique<Model>();

	tag_ = "player";
	CollisionManager::GetInstance()->AddCollision(this);

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 1,1,1 };

	sound_ = nullptr;

	time_ = 0;
}

Player::~Player()
{
	Destroy();
}

void Player::Initialize(Shader shader, GPipeline* pipeline)
{
	pipeline_ = pipeline;

	//ModelManager::GetInstance()->LoadModel("Resources\\Model\\Player\\Player.obj");

	//SetModel("Resources\\Model\\Player\\Player.obj");

	//model_->Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\Player\\Player.obj", pipeline_);
	model_->Initialize(MyDirectX::GetInstance(), shader, "Resources\\skydome\\skydome.obj", pipeline_);

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 10,10,10 };
	model_->mat_.rotAngle_ = { 0,0,0 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 0;
	model_->mat_.trans_.z_ = 0;

	controller_ = Controller::GetInstance();

	time_ = 0;

	sound_ = MyXAudio::GetInstance();
	//jumpSE_ = sound_->SoundLoadWave("Resources/sound/SE_jump.wav");

	//tex
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/skydome/skyBG.png");
}

void Player::Draw(size_t tex)
{
	//描画
	model_->Draw(tex);
}

void Player::Draw()
{
	//描画
	model_->Draw(tex_);
}

Vector2D Player::MoveCamera(Matrix matView, Matrix matProjection, Input* input)
{
	controller_->Update();

	Vector3D move = { 0 ,0 ,0 };

	move.x_ += input->GetKey(DIK_D) - input->GetKey(DIK_A);
	move.z_ += input->GetKey(DIK_W) - input->GetKey(DIK_S);

	model_->mat_.trans_ += move;

	model_->MatUpdate(matView, matProjection);

	return Vector2D(move.x_, move.z_);
}

void Player::Update(Matrix matView, Matrix matProjection)
{
	matView_ = matView;
	matProjection_ = matProjection;

	//コントローラーUpdate
	controller_->Update();

	//座標Update
	model_->MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 11;
}

void Player::SetDeadAnimation()
{
	model_->mat_.scale_ *= 2;
	model_->mat_.rotAngle_.y_ = 3.14f;
}

void Player::OnCollision()
{

}

void Player::SetCamera(Matrix matView, Matrix matProjection)
{
	//座標Update
	model_->MatUpdate(matView, matProjection);
}