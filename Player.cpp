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
	player.Initialize(dx_, shader, "Resources\\kyu\\kyu.obj", pipeline_);

	player.mat.Initialize();
	player.mat.scale = { 5,5,5 };
	player.mat.trans.x = 0;
	player.mat.trans.y = 10;
	player.mat.trans.z = 0;
}

void Player::Draw(int tex)
{
	player.Draw(tex);
}

void Player::Update(Matrix matView, Matrix matProjection, Input* input)
{
	player.mat.trans.x += input->GetKey(DIK_D) - input->GetKey(DIK_A);
	player.mat.trans.y -= input->GetKey(DIK_S) - input->GetKey(DIK_W);
	player.mat.trans.z -= input->GetKey(DIK_E) - input->GetKey(DIK_Q);

	player.MatUpdate(matView, matProjection);
}

void Player::Update(Matrix matView, Matrix matProjection, Controller* controller)
{
	Vector3D colVec = { 0,0,0 };

	colVec = { controller->GetLeftStickVec().x, -controller->GetLeftStickVec().y,0 };

	player.mat.trans += colVec;

	player.MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	player.mat.trans.x = 0;
	player.mat.trans.y = 0;
}
