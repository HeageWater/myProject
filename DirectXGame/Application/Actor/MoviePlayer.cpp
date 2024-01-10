#include "MoviePlayer.h"
#include "Easing.h"
#include <imgui.h>

MoviePlayer::MoviePlayer()
{
	player_.mat_.Initialize();
	player_.mat_.scale_ = { 3,3,3 };

	sound_ = nullptr;

	time_ = 0;
}

MoviePlayer::~MoviePlayer()
{
}

void MoviePlayer::Initialize(Shader shader, GPipeline* pipeline)
{
	pipeline_ = pipeline;

	player_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\Player\\Player.obj", pipeline_);

	player_.mat_.Initialize();
	player_.mat_.scale_ = { 3,3,3 };
	player_.mat_.rotAngle_ = { 0,0,0 };
	player_.mat_.trans_.x_ = 0;// 950;
	player_.mat_.trans_.y_ = 150;
	player_.mat_.trans_.z_ = 0;

	time_ = 100;

	sound_ = MyXAudio::GetInstance();

	vecFlag_ = false;
}

void MoviePlayer::Draw(size_t tex)
{
	player_.Draw(tex);
}

Vector2D MoviePlayer::MoveCamera(Matrix matView, Matrix matProjection, Input* input)
{
	Vector3D move = { 0 ,0 ,0 };

	move.x_ += input->GetKey(DIK_D) - input->GetKey(DIK_A);
	move.z_ += input->GetKey(DIK_W) - input->GetKey(DIK_S);

	player_.mat_.trans_ += move;

	player_.MatUpdate(matView, matProjection);

	return Vector2D(move.x_, move.z_);
}

void MoviePlayer::Update(Matrix matView, Matrix matProjection, Shader shader)
{
	matView_ = matView;
	matProjection_ = matProjection;

	const float pi = 3.1415f;

	time_++;

	//正面を向く
	player_.mat_.rotAngle_.y_ = pi;
	MaxTime = 1;
	MaxTime++;
	player_.mat_.trans_.y_ -= MaxTime;

	if (player_.mat_.trans_.y_ < 17.0f)
	{
		player_.mat_.trans_.y_ = 17.0f;
	}

	//座標Update
	player_.MatUpdate(matView, matProjection);
}

void MoviePlayer::Reset()
{
	player_.mat_.Initialize();
	player_.mat_.scale_ = { 3,3,3 };
	player_.mat_.trans_.x_ = 0;
	player_.mat_.trans_.y_ = 11;
	player_.mat_.trans_.z_ = 0;
}

void MoviePlayer::SetCamera(Matrix matView, Matrix matProjection)
{
	matView_ = matView;
	matProjection_ = matProjection;

	//座標Update
	player_.MatUpdate(matView, matProjection);
}

//GoalUpdate();
//GAmeOverUpdate();
//DeadAnimatinUpdate();
//StartUpdate();