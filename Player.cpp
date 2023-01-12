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
	attackModel.Initialize(dx_, shader, "Resources\\rasu\\rasu.obj", pipeline_);
	controller = Controller::GetInstance();

	attackModel.mat.scale = {5,5,5};

	jumpFlag = false;
	jumpPower = 0;
	GravityPower = 0;
}

void Player::Draw(int tex)
{
	player.Draw(tex);
	attackModel.Draw(tex);
}

void Player::Update(Matrix matView, Matrix matProjection)
{
	controller->Update();

	player.mat.trans.y -= GravityPower;

	GravityPower += 0.01f;

	/*if (controller->ButtonTriggerPush(A)) {
		if (jumpFlag == false) {
			jumpFlag = true;
			GravityPower = 0;
			jumpPower = 2.0f;
		}
	}*/

	Vector2D move = controller->GetLeftStickVec();
	move.normalize();

	if (move.x > 0.8f || move.x < -0.8f) {
		player.mat.trans.x -= 0.5f * move.x;
	}
	else {
		move.x = 0;
	}

	Jump();

	float Teihen = -32.0f;
	float Top = 5.0f;

	player.mat.trans.x = max(player.mat.trans.x,-52);
	player.mat.trans.x = min(player.mat.trans.x, 48);

	player.mat.trans.y = max(player.mat.trans.y, Teihen);
	//player.mat.trans.y = min(player.mat.trans.y, Top);

	attackRange = controller->GetRightStickVec();
	attackRange.normalize();

	float ad = 0.75f;

	if (attackRange.x < ad && attackRange.x > -ad &&
		attackRange.y < ad && attackRange.y > -ad) {
		attackRange.x = 1000;
	}

	//if (controller->StickInDeadZone(attackRange, Vector2D(0.9f, 0.9f))) {
	attackModel.mat.trans.x = -(15 * attackRange.x) + player.mat.trans.x;
	attackModel.mat.trans.y = -(15 * attackRange.y) + player.mat.trans.y;
	/*}
	else {
		attackModel.mat.trans.x = 0;
		attackModel.mat.trans.y = 1000;
	}*/

	player.MatUpdate(matView, matProjection);
	attackModel.MatUpdate(matView, matProjection);
}

void Player::Jump()
{
	if (jumpFlag) {
		jumpPower -= 0.02;

		player.mat.trans.y += jumpPower;

		if (jumpPower < 1.0f) {
			jumpFlag = false;
			jumpPower = 0.0f;
			GravityPower = 0.0f;
		}
	}
}

void Player::Attack()
{
}

void Player::CH()
{
}
