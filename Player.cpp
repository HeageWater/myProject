#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	player.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);
}

void Player::Draw(size_t tex)
{
	player.Draw(tex);
}

void Player::Update(Matrix matView, Matrix matProjection)
{
	player.MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	player.mat.trans.x = 0;
	player.mat.trans.y = 0;
}
