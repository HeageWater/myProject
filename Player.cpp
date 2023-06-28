#include "Player.h"

Player::Player()
{
	player.mat.Initialize();
	player.mat.scale = { 3,3,3 };
}

Player::~Player()
{

}

void Player::Initialize(MyDirectX* dx_, Shader shader, GPipeline* pipeline_)
{
	player.Initialize(dx_, shader, "Resources\\Model\\box.obj", pipeline_);

	player.mat.Initialize();
	player.mat.scale = { 10,10,10 };
	player.mat.trans.x = 0;
	player.mat.trans.y = 0;
	player.mat.trans.z = 0;
}

void Player::Draw(int tex)
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
