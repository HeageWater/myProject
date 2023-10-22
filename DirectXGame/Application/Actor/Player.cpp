#include "Player.h"
#include <imgui.h>

Player::Player()
{
	knockBackFlag_ = false;

	player_.mat.Initialize();
	player_.mat.scale = { 3,3,3 };

	playerAttack_.mat.Initialize();
	playerAttack_.mat.scale = { 3,3,3 };

	attackF_ = false;

	sound_ = nullptr;
	jumpSE_ = 0;

	life_ = 3;
	lesFlag_ = 0;
}

Player::~Player()
{
	for (size_t i = 0; i < attack_.size(); i++)
	{
		delete attack_[i];
	}
}

void Player::Initialize(Shader shader, GPipeline* pipeline_)
{
	player_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\Player\\Player.obj", pipeline_);

	player_.mat.Initialize();
	player_.mat.scale = { 3,3,3 };
	player_.mat.rotAngle = { 0,0,0 };
	player_.mat.trans.x = 0;// 950;
	player_.mat.trans.y = 11;
	player_.mat.trans.z = 0;

	playerAttack_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\box.obj", pipeline_);

	playerAttack_.mat.Initialize();
	playerAttack_.mat.scale = { 5,5,5 };
	playerAttack_.mat.trans = player_.mat.trans;

	controller_ = Controller::GetInstance();
	attackF_ = false;
	createAttackFlag_ = false;
	knockBackFlag_ = false;
	warpActionFlag_ = false;

	warpMord_ = 0;

	sound_ = MyXAudio::GetInstance();
	jumpSE_ = sound_->SoundLoadWave("Resources/sound/SE_jump.wav");
}

void Player::Draw(size_t tex, size_t tex2)
{
	if (lesFlag_ % 2 == 0)
	{
		player_.Draw(tex);
	}

	if (attackF_)
	{
		playerAttack_.Draw(tex2);
	}

	for (size_t i = 0; i < attack_.size(); i++)
	{
		attack_[i]->Draw();
	}
}

Vector2D Player::MoveCamera(Matrix matView, Matrix matProjection, Input* input)
{
	controller_->Update();

	Vector3D move = { 0 ,0 ,0 };

	move.x += input->GetKey(DIK_D) - input->GetKey(DIK_A);
	move.z += input->GetKey(DIK_W) - input->GetKey(DIK_S);

	player_.mat.trans += move;

	player_.MatUpdate(matView, matProjection);

	return Vector2D(move.x, move.z);
}

void Player::Update(Matrix matView, Matrix matProjection, Shader shader)
{
	//コントローラーUpdate
	controller_->Update();

	//左スティックの角度代入
	colVec_ = { controller_->GetLeftStickVec().x, 0,0 };

	//ワープしているか
	if (!WarpAction())
	{
		//ジャンプ
		//Jump();

		//ノックバック
		KnockBack();

		//座標に演算
		//player_.mat.trans += colVec;

		//攻撃
		Attack(shader);

		for (size_t i = 0; i < attack_.size(); i++)
		{
			attack_[i]->Update(matView, matProjection);

			if (attack_[i]->GetIsDead())
			{
				attack_.erase(attack_.begin() + i);
			}
		}
	}

	if (lesFlag_ > 0)
	{
		lesFlag_--;
	}

	//座標Update
	player_.MatUpdate(matView, matProjection);
	playerAttack_.MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	player_.mat.Initialize();
	player_.mat.scale = { 3,3,3 };
	player_.mat.trans.x = 0;// 950;
	player_.mat.trans.y = 11;
	player_.mat.trans.z = 0;

	playerAttack_.mat.Initialize();
	playerAttack_.mat.scale = { 5,5,5 };
	playerAttack_.mat.trans = player_.mat.trans;

	attackF_ = false;
	createAttackFlag_ = false;

	knockBackFlag_ = false;

	life_ = 3;
	lesFlag_ = 0;
}

void Player::Jump()
{
	float gravity = 0.2f;
	float maxGravity = 3;
	float jump = 1.0f;
	float maxJunp = 8;

	if (jumpPower_ > 0)
	{
		jumpPower_ -= jump;
	}

	if (gravirtPower_ < maxGravity)
	{
		gravirtPower_ += gravity;
	}

	if (controller_->ButtonTriggerPush(LT))
	{
		jumpPower_ = maxJunp;
		gravirtPower_ = 0;

		sound_->SoundPlayWave(jumpSE_);
	}

	player_.mat.trans.y += jumpPower_;
	player_.mat.trans.y -= gravirtPower_;
}

void Player::Attack(Shader shader)
{
	//playreの座標
	playerAttack_.mat.trans = player_.mat.trans;
	playerAttack_.mat.trans.x += 1000;

	//描画しない
	attackF_ = false;

	//右スティックの値
	Vector2D Rstick = controller_->GetRightStickVec();

	//最低ライン
	Vector2D minLine = { 0.2f,0.2f };

	//デッドゾ－ン超えてるか
	bool moreLineX = minLine.x < Rstick.x || -minLine.x > Rstick.x;
	bool moreLineY = minLine.y < Rstick.y || -minLine.y > Rstick.y;

	//デッドゾ－ン超えてるか
	if (moreLineX || moreLineY)
	{
		playerAttack_.mat.trans.x -= 1000;

		//normalize
		Rstick.normalize();

		//移動値
		float move = 30;
		Vector3D movePos = { move * Rstick.x, -(move * Rstick.y),0 };

		//移送
		playerAttack_.mat.trans += movePos;
		playerAttack_.mat.rotAngle.z = movePos.normalize().x;

		//描画する
		attackF_ = true;

		////attackModel生成
		//if (createAttackFlag)
		//{
		//	PlayerAttack* newAttack = new PlayerAttack(dx_, shader, pipeline_);

		//	Vector3D pos = player_.mat.trans;
		//	newAttack->SetPos(pos);

		//	Vector2D vec = controller->GetRightStickVec();
		//	vec *= -1;
		//	newAttack->SetVec(vec);

		//	newAttack->SetUpdate();

		//	attack.push_back(newAttack);

		//	createAttackFlag = false;
		//}
	}
	else if (!createAttackFlag_)
	{
		//attackModel生成
		createAttackFlag_ = true;
	}
}

void Player::PopPlayerAttack()
{
}

bool Player::CollisionAttackToEnemy(Model enemy)
{
	for (size_t i = 0; i < attack_.size(); i++)
	{
		Vector3D nowPos = attack_[i]->GetPos();
		Vector3D nowScale = attack_[i]->GetScale();

		float a = (enemy.mat.trans.x - nowPos.x) * (enemy.mat.trans.x - nowPos.x);
		float b = (enemy.mat.trans.y - nowPos.y) * (enemy.mat.trans.y - nowPos.y);

		float c = enemy.mat.scale.x * nowScale.x;

		//あたり判定
		if (a + b < c)
		{
			attack_.erase(attack_.begin() + i);
			return true;
		}
	}
	return false;
}

void Player::LesLife()
{
	life_--;
	lesFlag_ = 50;
	knockBackFlag_ = true;
}

void Player::KnockBack()
{
	if (knockBackFlag_)
	{
		float n = knockBackVec_ / 10;

		player_.mat.trans.x += knockBackVec_;
		player_.mat.trans.y += n;

		if (knockBackVec_ > 0)
		{
			knockBackVec_ -= 0.1f;

			if (knockBackVec_ < 0)
			{
				knockBackVec_ = 0;
			}
		}
		else
		{
			knockBackVec_ += 0.1f;

			if (knockBackVec_ > 0)
			{
				knockBackVec_ = 0;
			}
		}

		if (knockBackVec_ == 0)
		{
			knockBackFlag_ = false;
		}
	}
}

bool Player::StageCollsion(Model stage, Matrix matView, Matrix matProjection)
{
	if (player_.mat.scale.x == 0)
	{
		return false;
	}

	float DisX = player_.mat.trans.x - stage.mat.trans.x;
	float DisY = player_.mat.trans.y - stage.mat.trans.y;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= player_.mat.scale.x + stage.mat.scale.x &&
		DisY <= player_.mat.scale.y + stage.mat.scale.y)
	{
		if (jumpPower_ > 0)
		{
			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.y -= 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}

		if (gravirtPower_ > 0)
		{
			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.y += 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}

		if (colVec_.x > 0)
		{
			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.x -= 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}
		else
		{
			//player_.mat.trans += colVec;

			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.x += 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}

		player_.MatUpdate(matView, matProjection);

		return true;
	}

	player_.MatUpdate(matView, matProjection);

	return false;
}

bool Player::StageCollsionX(Model stage)
{
	if (player_.mat.scale.x == 0)
	{
		return false;
	}

	float DisX = player_.mat.trans.x - stage.mat.trans.x;
	float DisY = player_.mat.trans.y - stage.mat.trans.y;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= player_.mat.scale.x + stage.mat.scale.x &&
		DisY <= player_.mat.scale.y + stage.mat.scale.y)
	{
		if (colVec_.x > 0)
		{
			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.x -= 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}
		else
		{
			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.x += 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}

		return true;
	}

	return false;
}

bool Player::StageCollsionY(Model stage)
{
	if (player_.mat.scale.x == 0)
	{
		return false;
	}

	float DisX = player_.mat.trans.x - stage.mat.trans.x;
	float DisY = player_.mat.trans.y - stage.mat.trans.y;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= player_.mat.scale.x + stage.mat.scale.x &&
		DisY <= player_.mat.scale.y + stage.mat.scale.y)
	{
		if (jumpPower_ > 0)
		{
			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.y -= 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}

		if (gravirtPower_ > 0)
		{
			bool colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
			bool colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;

			while (colX && colY)
			{
				player_.mat.trans.y += 1;

				DisX = player_.mat.trans.x - stage.mat.trans.x;
				DisY = player_.mat.trans.y - stage.mat.trans.y;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat.scale.x + stage.mat.scale.x;
				colY = DisY <= player_.mat.scale.y + stage.mat.scale.y;
			}
		}

		return true;
	}

	return false;
}

bool Player::StageCollision(Model stage)
{
	MoveX();
	bool X = StageCollsionX(stage);

	MoveY();
	bool Y = StageCollsionY(stage);

	return X || Y;
}

bool Player::PlayerCollision(Model enemy)
{
	Vector3D nowPos = GetPos();
	Vector3D nowScale = GetScale();

	float a = (enemy.mat.trans.x - nowPos.x) * (enemy.mat.trans.x - nowPos.x);
	float b = (enemy.mat.trans.y - nowPos.y) * (enemy.mat.trans.y - nowPos.y);

	float c = enemy.mat.scale.x * nowScale.x;

	//あたり判定
	if (a + b < c)
	{
		if (lesFlag_ == 0)
		{
			LesLife();
			knockBackVec_ = nowPos.x - enemy.mat.trans.x;
			return true;
		}
	}
	return false;
}

bool Player::WarpAction()
{
	if (warpActionFlag_)
	{
		colVec_ = { 0,0,0 };

		float spd = 0.3f;
		float Ten = 10;
		float Zero = 0;
		float One = 1;
		float Two = 2;
		float Three = 3;
		float distance = 0.7f;
		float scale = 0.1f;

		player_.mat.rotAngle.z += spd;

		if (warpMord_ == (uint32_t)Zero)
		{
			if (player_.mat.trans.x > warpPos_[(uint32_t)Zero].x - Ten)
			{
				player_.mat.trans.x -= distance;
			}
			else
			{
				warpMord_ = (uint32_t)One;
			}
		}
		else if (warpMord_ == (uint32_t)One)
		{
			player_.mat.scale -= {scale, scale, scale};

			if (player_.mat.trans.x < warpPos_[(uint32_t)Zero].x)
			{
				player_.mat.trans.x += distance;
			}
			else
			{
				warpMord_ = (uint32_t)Two;
			}
		}
		else if (warpMord_ == (uint32_t)Two)
		{
			player_.mat.scale = { Zero,Zero,Zero };

			player_.mat.trans.x += Two;
		}
		else if (warpMord_ == (uint32_t)Three)
		{
			player_.mat.rotAngle.z = Zero;

			if (player_.mat.scale.x < Three)
			{
				player_.mat.scale += {scale, scale, scale};
			}
			else
			{
				player_.mat.scale = { Three,Three,Three };
				warpMord_ = (uint32_t)Zero;
				warpActionFlag_ = false;
			}
		}

		return true;
	}

	return false;
}
