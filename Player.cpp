#include "Player.h"

Player::Player()
{
	player_.mat_.Initialize();
	player_.mat_.trans_ = { 3,3,3 };
}

Player::~Player()
{

}

void Player::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	player_.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
}

void Player::Draw(size_t tex)
{
	player_.Draw(tex);
}

void Player::Update(Matrix matView, Matrix matProjection)
{
	player_.MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	player_.mat_.trans_.x_ = 0;
	player_.mat_.trans_.y_ = 0;
}
