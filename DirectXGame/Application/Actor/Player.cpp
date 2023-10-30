#include "Player.h"
#include <imgui.h>

Player::Player()
{
	knockBackFlag_ = false;

	player_.mat_.Initialize();
	player_.mat_.scale_ = { 3,3,3 };

	playerAttack_.mat_.Initialize();
	playerAttack_.mat_.scale_ = { 3,3,3 };

	attackF_ = false;

	sound_ = nullptr;
	jumpSE_ = 0;

	life_ = 3;
	lesFlag_ = 0;

	sheikF_ = false;
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

	player_.mat_.Initialize();
	player_.mat_.scale_ = { 3,3,3 };
	player_.mat_.rotAngle_ = { 0,0,0 };
	player_.mat_.trans_.x_ = 0;// 950;
	player_.mat_.trans_.y_ = 11;
	player_.mat_.trans_.z_ = 0;

	playerAttack_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\box.obj", pipeline_);

	playerAttack_.mat_.Initialize();
	playerAttack_.mat_.scale_ = { 5,5,5 };
	playerAttack_.mat_.trans_ = player_.mat_.trans_;

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

	move.x_ += input->GetKey(DIK_D) - input->GetKey(DIK_A);
	move.z_ += input->GetKey(DIK_W) - input->GetKey(DIK_S);

	player_.mat_.trans_ += move;

	player_.MatUpdate(matView, matProjection);

	return Vector2D(move.x_, move.z_);
}

void Player::Update(Matrix matView, Matrix matProjection, Shader shader)
{
	//コントローラーUpdate
	controller_->Update();

	//左スティックの角度代入
	colVec_ = { controller_->GetLeftStickVec().x_, 0,0 };

	//ワープしているか
	if (!WarpAction())
	{
		//ジャンプ
		Jump();

		//ノックバック
		KnockBack();

		//座標に演算
		//player_.mat.trans += colVec;

		//攻撃
		Attack(shader);

		//シェイク
		bool sheik = sheikF_ > 0;
		if (sheik)
		{
			Sheik();
		}

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
	player_.mat_.Initialize();
	player_.mat_.scale_ = { 3,3,3 };
	player_.mat_.trans_.x_ = 0;// 950;
	player_.mat_.trans_.y_ = 11;
	player_.mat_.trans_.z_ = 0;

	playerAttack_.mat_.Initialize();
	playerAttack_.mat_.scale_ = { 5,5,5 };
	playerAttack_.mat_.trans_ = player_.mat_.trans_;

	attackF_ = false;
	createAttackFlag_ = false;

	knockBackFlag_ = false;

	life_ = 3;
	lesFlag_ = 0;
}

void Player::Jump()
{
	float gravity = 0.2f / 2;
	float maxGravity = 3 / 2;
	float jump = 1.0f / 2;
	float maxJunp = 8 / 2;

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

	player_.mat_.trans_.y_ += jumpPower_;
	player_.mat_.trans_.y_ -= gravirtPower_;
}

void Player::Attack(Shader shader)
{
	//playreの座標
	playerAttack_.mat_.trans_ = player_.mat_.trans_;
	playerAttack_.mat_.trans_.x_ += 1000;

	//描画しない
	attackF_ = false;

	//右スティックの値
	Vector2D Rstick = controller_->GetRightStickVec();

	//最低ライン
	Vector2D minLine = { 0.2f,0.2f };

	//デッドゾ－ン超えてるか
	bool moreLineX = minLine.x_ < Rstick.x_ || -minLine.x_ > Rstick.x_;
	bool moreLineY = minLine.y_ < Rstick.y_ || -minLine.y_ > Rstick.y_;

	//デッドゾ－ン超えてるか
	if (moreLineX || moreLineY)
	{
		playerAttack_.mat_.trans_.x_ -= 1000;

		//normalize
		Rstick.normalize();

		//移動値
		float move = 30;
		Vector3D movePos = { move * Rstick.x_, -(move * Rstick.y_),0 };

		//移送
		playerAttack_.mat_.trans_ += movePos;
		playerAttack_.mat_.rotAngle_.z_ = movePos.normalize().x_;

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

		float a = (enemy.mat_.trans_.x_ - nowPos.x_) * (enemy.mat_.trans_.x_ - nowPos.x_);
		float b = (enemy.mat_.trans_.y_ - nowPos.y_) * (enemy.mat_.trans_.y_ - nowPos.y_);

		float c = enemy.mat_.scale_.x_ * nowScale.x_;

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

		player_.mat_.trans_.x_ += knockBackVec_;
		player_.mat_.trans_.y_ += n;

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
	if (player_.mat_.scale_.x_ == 0)
	{
		return false;
	}

	float DisX = player_.mat_.trans_.x_ - stage.mat_.trans_.x_;
	float DisY = player_.mat_.trans_.y_ - stage.mat_.trans_.y_;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_ &&
		DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_)
	{
		if (jumpPower_ > 0)
		{
			bool colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
			bool colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;

			while (colX && colY)
			{
				player_.mat_.trans_.y_ -= 1;

				DisX = player_.mat_.trans_.x_ - stage.mat_.trans_.x_;
				DisY = player_.mat_.trans_.y_ - stage.mat_.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
				colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;
			}
		}

		if (gravirtPower_ > 0)
		{
			bool colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
			bool colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;

			while (colX && colY)
			{
				player_.mat_.trans_.y_ += 1;

				DisX = player_.mat_.trans_.x_ - stage.mat_.trans_.x_;
				DisY = player_.mat_.trans_.y_ - stage.mat_.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
				colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;
			}
		}

		if (colVec_.x_ > 0)
		{
			bool colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
			bool colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;

			while (colX && colY)
			{
				player_.mat_.trans_.x_ -= 1;

				DisX = player_.mat_.trans_.x_ - stage.mat_.trans_.x_;
				DisY = player_.mat_.trans_.y_ - stage.mat_.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
				colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;
			}
		}
		else
		{
			//player_.mat.trans += colVec;

			bool colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
			bool colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;

			while (colX && colY)
			{
				player_.mat_.trans_.x_ += 1;

				DisX = player_.mat_.trans_.x_ - stage.mat_.trans_.x_;
				DisY = player_.mat_.trans_.y_ - stage.mat_.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= player_.mat_.scale_.x_ + stage.mat_.scale_.x_;
				colY = DisY <= player_.mat_.scale_.y_ + stage.mat_.scale_.y_;
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
	if (player_.mat_.scale_.x_ == 0)
	{
		return false;
	}

	float playerTransX = player_.mat_.trans_.x_;
	float playerTransY = player_.mat_.trans_.y_;
	float playerScaleX = player_.mat_.scale_.x_ * 2;
	float playerScaleY = player_.mat_.scale_.y_ * 2;

	float stageTransX = stage.mat_.trans_.x_;
	float stageTransY = stage.mat_.trans_.y_;
	float stageScaleX = stage.mat_.scale_.x_;
	float stageScaleY = stage.mat_.scale_.y_;

	float DisX = playerTransX - stageTransX;
	float DisY = playerTransY - stageTransY;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= playerScaleX + stageScaleX &&
		DisY <= playerScaleY + stageScaleY)
	{
		if (colVec_.x_ > 0)
		{
			bool colX = DisX <= playerScaleX + stageScaleX;
			bool colY = DisY <= playerScaleY + stageScaleY;

			while (colX && colY)
			{
				player_.mat_.trans_.x_ -= 1;

				playerTransX = player_.mat_.trans_.x_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;
			}
		}
		else
		{
			bool colX = DisX <= playerScaleX + stageScaleX;
			bool colY = DisY <= playerScaleY + stageScaleY;

			while (colX && colY)
			{
				player_.mat_.trans_.x_ += 1;

				playerTransX = player_.mat_.trans_.x_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;
			}
		}

		return true;
	}

	return false;
}

bool Player::StageCollsionY(Model stage)
{
	if (player_.mat_.scale_.x_ == 0)
	{
		return false;
	}

	float playerTransX = player_.mat_.trans_.x_;
	float playerTransY = player_.mat_.trans_.y_;
	float playerScaleX = player_.mat_.scale_.x_ * 2;
	float playerScaleY = player_.mat_.scale_.y_ * 2;

	float stageTransX = stage.mat_.trans_.x_;
	float stageTransY = stage.mat_.trans_.y_;
	float stageScaleX = stage.mat_.scale_.x_;
	float stageScaleY = stage.mat_.scale_.y_;

	float DisX = playerTransX - stageTransX;
	float DisY = playerTransY - stageTransY;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= playerScaleX + stageScaleX &&
		DisY <= playerScaleY + stageScaleY)
	{
		if (jumpPower_ > 0)
		{
			bool colX = DisX <= playerScaleX + stageScaleX;
			bool colY = DisY <= playerScaleY + stageScaleY;

			while (colX && colY)
			{
				player_.mat_.trans_.y_ -= 1;

				playerTransY = player_.mat_.trans_.y_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;
			}
		}

		if (gravirtPower_ > 0)
		{
			bool colX = DisX <= playerScaleX + stageScaleX;
			bool colY = DisY <= playerScaleY + stageScaleY;

			while (colX && colY)
			{
				player_.mat_.trans_.y_ += 1;

				playerTransY = player_.mat_.trans_.y_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;
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

	float a = (enemy.mat_.trans_.x_ - nowPos.x_) * (enemy.mat_.trans_.x_ - nowPos.x_);
	float b = (enemy.mat_.trans_.y_ - nowPos.y_) * (enemy.mat_.trans_.y_ - nowPos.y_);

	float c = enemy.mat_.scale_.x_ * nowScale.x_;

	//あたり判定
	if (a + b < c)
	{
		if (lesFlag_ == 0)
		{
			LesLife();
			knockBackVec_ = nowPos.x_ - enemy.mat_.trans_.x_;
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

		player_.mat_.rotAngle_.z_ += spd;

		if (warpMord_ == (uint32_t)Zero)
		{
			if (player_.mat_.trans_.x_ > warpPos_[(uint32_t)Zero].x_ - Ten)
			{
				player_.mat_.trans_.x_ -= distance;
			}
			else
			{
				warpMord_ = (uint32_t)One;
			}
		}
		else if (warpMord_ == (uint32_t)One)
		{
			player_.mat_.scale_ -= {scale, scale, scale};

			if (player_.mat_.trans_.x_ < warpPos_[(uint32_t)Zero].x_)
			{
				player_.mat_.trans_.x_ += distance;
			}
			else
			{
				warpMord_ = (uint32_t)Two;
			}
		}
		else if (warpMord_ == (uint32_t)Two)
		{
			player_.mat_.scale_ = { Zero,Zero,Zero };

			player_.mat_.trans_.x_ += Two;
		}
		else if (warpMord_ == (uint32_t)Three)
		{
			player_.mat_.rotAngle_.z_ = Zero;

			if (player_.mat_.scale_.x_ < Three)
			{
				player_.mat_.scale_ += {scale, scale, scale};
			}
			else
			{
				player_.mat_.scale_ = { Three,Three,Three };
				warpMord_ = (uint32_t)Zero;
				warpActionFlag_ = false;
			}
		}

		return true;
	}

	return false;
}


void Player::Sheik()
{
	float x = (float)MyMath::GetRandom(10, 20);
	float y = (float)MyMath::GetRandom(2, 3);

	if (sheikF_ % 2 == 0)
	{
		x = -x;
		y = -y;
	}

	player_.mat_.trans_.x_ += x;
	player_.mat_.trans_.y_ += y;

	if (sheikF_ > 0)
	{
		sheikF_--;
	}
}

void Player::SetCamera(Matrix matView, Matrix matProjection)
{
	//座標Update
	player_.MatUpdate(matView, matProjection);
	playerAttack_.MatUpdate(matView, matProjection);
}