#include "Player.h"
#include "Easing.h"
#include "CollisionManager.h"
#include "ModelManager.h"
#include <imgui.h>

Player::Player()
{
	//modelを制作
	model_ = std::make_unique<Model>();
	donatu_ = std::make_unique<Model>();

	//tagを追加
	tag_ = "player";

	//コリジョンマネージャーに登録
	CollisionManager::GetInstance()->AddCollision(this);

	knockBackFlag_ = false;

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };

	attackF_ = false;

	sound_ = nullptr;
	jumpSE_ = 0;

	life_ = 3;
	lesFlag_ = 0;

	sheikF_ = false;

	deadAnimationMode_ = 0;

	time_ = 0;
}

Player::~Player()
{
	for (size_t i = 0; i < attack_.size(); i++)
	{
		delete attack_[i];
	}

	Destroy();
}

void Player::Initialize(Shader shader, GPipeline* pipeline)
{
	pipeline_ = pipeline;

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("Resources\\Model\\Player\\Player.obj");

	//モデルセット
	model_ = SetModel("Resources\\Model\\Player\\Player.obj");

	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };
	model_->mat_.rotAngle_ = { 0,0,0 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 11;
	model_->mat_.trans_.z_ = 0;

	controller_ = Controller::GetInstance();
	attackF_ = false;
	createAttackFlag_ = false;
	knockBackFlag_ = false;
	warpActionFlag_ = false;

	warpMord_ = 0;
	time_ = 0;

	sound_ = MyXAudio::GetInstance();
	jumpSE_ = sound_->SoundLoadWave("Resources/sound/SE_jump.wav");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("Resources\\Model\\donatu\\rasu.obj");

	//モデルセット
	donatu_ = SetModel("Resources\\Model\\donatu\\rasu.obj");
	donatu_->mat_.scale_ = { 3,3,3 };

	//
	whiteTex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources\\sprite\\white1x1.png");
}

void Player::Draw(size_t tex)
{
	if (lesFlag_ % 2 == 0)
	{
		model_->Draw(tex);
	}

	for (size_t i = 0; i < attack_.size(); i++)
	{
		attack_[i]->Draw();
	}

	if (jumpAnimationF_)
	{
		donatu_->Draw(whiteTex_);
	}
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

void Player::Update(Matrix matView, Matrix matProjection, Shader shader)
{
	matView_ = matView;
	matProjection_ = matProjection;

	//コントローラーUpdate
	controller_->Update();

	//左スティックの角度代入
	colVec_ = { controller_->GetLeftStickVec(Vector2D(0.3f,0.3f)).x_, 0,0 };

	float line = 0.3f;

	bool prus = colVec_.x_ >= 0.0f;

	if (colVec_.x_ > -line && colVec_.x_ < line)
	{
		colVec_.x_ = 0.0f;
	}

	//ワープしているか
	if (!WarpAction())
	{
		if (prus)
		{
			model_->mat_.rotAngle_.y_ = 2;
		}
		else
		{
			model_->mat_.rotAngle_.y_ = -2;
		}

		//ジャンプ
		Jump();

		//壁キック
		WallRightKick();
		WallLeftKick();

		//壁キック分加算
		colVec_ += kickVec_;

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
			attack_[i]->Update(matView_, matProjection_);

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
	model_->MatUpdate(matView, matProjection);
	donatu_->MatUpdate(matView, matProjection);
}

void Player::Reset()
{
	model_->mat_.Initialize();
	model_->mat_.scale_ = { 3,3,3 };
	model_->mat_.trans_.x_ = 0;// 950;
	model_->mat_.trans_.y_ = 11;
	model_->mat_.trans_.z_ = 0;

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
	float maxJunp = 6;
	float maxJunpCount = 2;

	if (jumpPower_ > 0)
	{
		jumpPower_ -= jump;
	}

	if (gravityPower_ < maxGravity)
	{
		gravityPower_ += gravity;
	}

	//壁に擦っているか
	bool wallKick = rightKick_ || leftKick_;

	if (!wallKick)
	{
		if (controller_->ButtonTriggerPush(LT))
		{
			if (jumpCount < maxJunpCount)
			{
				//跳躍力を最大に
				jumpPower_ = maxJunp;

				//重力を0に
				gravityPower_ = 0;

				//連続ジャンプ数
				jumpCount++;

				//ジャンプ音
				sound_->SoundPlayWave(jumpSE_);

				//2回目のジャンプ時にアニメーション
				if (jumpCount == maxJunpCount)
				{
					//ジャンプアニメーションフラグをonに
					jumpAnimationF_ = true;

					//ドーナツ型のモデルの場所をplayerの下に
					donatu_->mat_.trans_ = model_->mat_.trans_;
					donatu_->mat_.trans_.y_ += 1;
					donatu_->mat_.scale_ = { 1,1,1 };
					donatu_->mat_.rotAngle_.x_ = 1.3f;

					time_ = 0;
				}
			}
		}
	}

	if (wallKick)
	{
		//重力と跳躍力を加算
		model_->mat_.trans_.y_ += jumpPower_;
		model_->mat_.trans_.y_ -= gravityPower_ * 0.7f;
	}
	else
	{

		//重力と跳躍力を加算
		model_->mat_.trans_.y_ += jumpPower_;
		model_->mat_.trans_.y_ -= gravityPower_;
	}

	//ジャンプアニメーション
	JumpAnimation();
}

void Player::Attack(Shader shader)
{
	//描画しない
	attackF_ = false;

	//右スティックの値
	Vector2D Rstick = controller_->GetRightStickVec();

	//最低ライン
	Vector2D minLine = { 0.75f,0.75f };

	//デッドゾ－ン超えてるか
	bool moreLineX = minLine.x_ < Rstick.x_ || -minLine.x_ > Rstick.x_;
	bool moreLineY = minLine.y_ < Rstick.y_ || -minLine.y_ > Rstick.y_;

	//デッドゾ－ン超えてるか
	if (moreLineX || moreLineY)
	{
		//normalize
		Rstick.normalize();

		//移動値
		float move = 30;
		Vector3D movePos = { move * Rstick.x_, -(move * Rstick.y_),0 };

		//描画する
		attackF_ = true;

		//attackModel生成
		if (createAttackFlag_)
		{
			PlayerAttack* newAttack = new PlayerAttack(MyDirectX::GetInstance(), shader, pipeline_);

			Vector3D pos = model_->mat_.trans_;
			newAttack->SetPos(pos);

			Vector3D vec = { controller_->GetRightStickVec().x_,controller_->GetRightStickVec().y_,0 };
			vec *= -1;
			newAttack->SetVec(vec);

			newAttack->SetUpdate();

			attack_.push_back(newAttack);

			createAttackFlag_ = false;
		}
	}
	else if (!createAttackFlag_)
	{
		//attackModel生成
		createAttackFlag_ = true;
	}
}

void Player::WallRightKick()
{
	float moveX = 6;
	float moveY = 6;
	float gen = 0.5f;

	if (kickVec_.x_ > 0)
	{
		kickVec_.x_ -= gen;

		if (kickVec_.x_ <= 0)
		{
			kickVec_.x_ = 0;
		}
	}

	if (kickVec_.y_ > 0)
	{
		kickVec_.y_ -= gen / 2;

		if (kickVec_.y_ <= 0)
		{
			kickVec_.y_ = 0;
		}
	}

	//
	if (!rightKick_ || jumpCount == 0)
	{
		return;
	}

	if (controller_->ButtonTriggerPush(LT))
	{
		kickVec_.x_ = moveX;
		kickVec_.y_ = moveY;
		rightKick_ = false;
	}

	rightKick_ = false;
}

void Player::WallLeftKick()
{
	float moveX = 6;
	float moveY = 6;
	float gen = 0.5f;

	if (kickVec_.x_ < 0)
	{
		kickVec_.x_ += gen;

		if (kickVec_.x_ >= 0)
		{
			kickVec_.x_ = 0;
		}
	}

	if (kickVec_.y_ > 0)
	{
		kickVec_.y_ -= gen / 2;

		if (kickVec_.y_ <= 0)
		{
			kickVec_.y_ = 0;
		}
	}

	//
	if (!leftKick_ || jumpCount == 0)
	{
		return;
	}

	if (controller_->ButtonTriggerPush(LT))
	{
		kickVec_.x_ = -moveX;
		kickVec_.y_ = moveY;
		leftKick_ = false;
	}

	leftKick_ = false;
}

void Player::SetDeadAnimation()
{
	knockBackVec_ = 0;
	deadAnimationMode_ = 1;
	deadAnimationPos_ = model_->mat_.trans_;
	model_->mat_.scale_ *= 2;
	model_->mat_.rotAngle_.y_ = 3.14f;
}

void Player::OnCollision()
{
	LesLife();
}

void Player::JumpAnimation()
{
	if (jumpAnimationF_)
	{
		time_++;

		model_->mat_.rotAngle_.x_ += 0.3f;

		//donatu_->mat_.scale_.x_ = (float)Easing::EaseIn(static_cast<double>(1), static_cast<double>(10), static_cast<double>(time_ / 10), static_cast<float>(10));
		//donatu_->mat_.scale_.y_ = (float)Easing::EaseIn(static_cast<double>(1), static_cast<double>(3), static_cast<double>(time_ / 10), static_cast<float>(10));

		donatu_->mat_.scale_.x_ += 0.5f;
		donatu_->mat_.scale_.y_ += 0.1f;

		if (model_->mat_.rotAngle_.x_ > 5.5f)
		{
			model_->mat_.rotAngle_.x_ = 0;
			jumpAnimationF_ = false;
		}
	}
}

bool Player::DeadAnimation()
{
	float maxTime = 30;
	time_++;

	switch (deadAnimationMode_)
	{
	case 1:
		if (time_ > maxTime)
		{
			deadAnimationMode_ += 2;
		}

		if (model_->mat_.trans_.z_ >= -50.0f)
		{
			model_->mat_.trans_.z_ -= 2.0f;
		}

		break;
	case 2:
		model_->mat_.trans_.z_ = -50.0f;
		model_->mat_.rotAngle_.z_ += 0.15f;

		model_->mat_.trans_.y_ += model_->mat_.rotAngle_.z_ / 3;

		if (time_ > maxTime * deadAnimationMode_)
		{
			deadAnimationMode_++;
		}
		break;
	case 3:
		model_->mat_.rotAngle_.z_ += 0.15f;
		model_->mat_.trans_.y_ -= model_->mat_.rotAngle_.z_ / 3;

		if (time_ > maxTime * deadAnimationMode_ + 10)
		{
			return true;
		}
		break;
	default:


		break;
	}

	//座標Update
	model_->MatUpdate(matView_, matProjection_);

	return false;
}

void Player::PopPlayerAttack()
{
}

bool Player::CollisionAttackToEnemy(Model enemy)
{
	for (size_t i = 0; i < attack_.size(); i++)
	{
		Vector3D nowPos = attack_[i]->GetPos();
		Vector3D nowScale = attack_[i]->GetScale() * 3;

		float a = (enemy.mat_.trans_.x_ - nowPos.x_) * (enemy.mat_.trans_.x_ - nowPos.x_);
		float b = (enemy.mat_.trans_.y_ - nowPos.y_) * (enemy.mat_.trans_.y_ - nowPos.y_);

		float c = enemy.mat_.scale_.x_ * nowScale.x_;

		//あたり判定
		if (a + b < c)
		{
			attack_.erase(attack_.begin() + i);
			return true;
		}

		nowPos = -attack_[i]->GetPos();
		nowScale = attack_[i]->GetScale() * 3;

		a = (enemy.mat_.trans_.x_ - nowPos.x_) * (enemy.mat_.trans_.x_ - nowPos.x_);
		b = (enemy.mat_.trans_.y_ - nowPos.y_) * (enemy.mat_.trans_.y_ - nowPos.y_);

		c = enemy.mat_.scale_.x_ * nowScale.x_;

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

	if (life_ > 0)
	{
		lesFlag_ = 50;
		knockBackFlag_ = true;
	}
}

void Player::KnockBack()
{
	if (knockBackFlag_)
	{
		float n = knockBackVec_ / 10;

		model_->mat_.trans_.x_ += knockBackVec_;
		model_->mat_.trans_.y_ += n;

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

bool Player::StageCollsion(MyMath::ObjMatrix stage, Matrix matView, Matrix matProjection)
{
	if (model_->mat_.scale_.x_ == 0)
	{
		return false;
	}

	float DisX = model_->mat_.trans_.x_ - stage.trans_.x_;
	float DisY = model_->mat_.trans_.y_ - stage.trans_.y_;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= model_->mat_.scale_.x_ + stage.scale_.x_ &&
		DisY <= model_->mat_.scale_.y_ + stage.scale_.y_)
	{
		if (jumpPower_ > 0)
		{
			bool colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
			bool colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;

			while (colX && colY)
			{
				model_->mat_.trans_.y_ -= 1;

				DisX = model_->mat_.trans_.x_ - stage.trans_.x_;
				DisY = model_->mat_.trans_.y_ - stage.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
				colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;
			}
		}

		if (gravityPower_ > 0)
		{
			bool colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
			bool colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;

			while (colX && colY)
			{
				model_->mat_.trans_.y_ += 1;

				DisX = model_->mat_.trans_.x_ - stage.trans_.x_;
				DisY = model_->mat_.trans_.y_ - stage.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
				colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;
			}
		}

		if (colVec_.x_ > 0)
		{
			bool colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
			bool colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;

			while (colX && colY)
			{
				model_->mat_.trans_.x_ -= 1;

				DisX = model_->mat_.trans_.x_ - stage.trans_.x_;
				DisY = model_->mat_.trans_.y_ - stage.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
				colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;
			}
		}
		else
		{
			//player_.mat.trans += colVec;

			bool colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
			bool colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;

			while (colX && colY)
			{
				model_->mat_.trans_.x_ += 1;

				DisX = model_->mat_.trans_.x_ - stage.trans_.x_;
				DisY = model_->mat_.trans_.y_ - stage.trans_.y_;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= model_->mat_.scale_.x_ + stage.scale_.x_;
				colY = DisY <= model_->mat_.scale_.y_ + stage.scale_.y_;
			}
		}

		model_->MatUpdate(matView, matProjection);

		return true;
	}

	model_->MatUpdate(matView, matProjection);

	return false;
}

bool Player::StageCollsionX(MyMath::ObjMatrix stage)
{
	if (model_->mat_.scale_.x_ == 0)
	{
		return false;
	}

	float playerTransX = model_->mat_.trans_.x_;
	float playerTransY = model_->mat_.trans_.y_;
	float playerScaleX = model_->mat_.scale_.x_ * 2;
	float playerScaleY = model_->mat_.scale_.y_ * 2;

	float stageTransX = stage.trans_.x_;
	float stageTransY = stage.trans_.y_;
	float stageScaleX = stage.scale_.x_;
	float stageScaleY = stage.scale_.y_;

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
				model_->mat_.trans_.x_ -= 1;

				playerTransX = model_->mat_.trans_.x_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;

				//leftKick
				if (jumpCount != 0)
				{
					leftKick_ = true;
				}
			}
		}
		else
		{
			bool colX = DisX <= playerScaleX + stageScaleX;
			bool colY = DisY <= playerScaleY + stageScaleY;

			while (colX && colY)
			{
				model_->mat_.trans_.x_ += 1;

				playerTransX = model_->mat_.trans_.x_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;

				//rightKick
				if (jumpCount != 0)
				{
					rightKick_ = true;
				}
			}
		}

		return true;
	}

	return false;
}

bool Player::StageCollsionY(MyMath::ObjMatrix stage)
{
	if (model_->mat_.scale_.x_ == 0)
	{
		return false;
	}

	float playerTransX = model_->mat_.trans_.x_;
	float playerTransY = model_->mat_.trans_.y_;
	float playerScaleX = model_->mat_.scale_.x_ * 2;
	float playerScaleY = model_->mat_.scale_.y_ * 2;

	float stageTransX = stage.trans_.x_;
	float stageTransY = stage.trans_.y_;
	float stageScaleX = stage.scale_.x_;
	float stageScaleY = stage.scale_.y_;

	float DisX = playerTransX - stageTransX;
	float DisY = playerTransY - stageTransY;

	DisX = abs(DisX);
	DisY = abs(DisY);

	if (DisX <= playerScaleX + stageScaleX &&
		DisY <= playerScaleY + stageScaleY)
	{
		if (jumpPower_ > 0 || kickVec_.y_ > 0)
		{
			bool colX = DisX <= playerScaleX + stageScaleX;
			bool colY = DisY <= playerScaleY + stageScaleY;

			while (colX && colY)
			{
				model_->mat_.trans_.y_ -= 1;

				playerTransY = model_->mat_.trans_.y_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;
			}
		}

		if (gravityPower_ > 0)
		{
			bool colX = DisX <= playerScaleX + stageScaleX;
			bool colY = DisY <= playerScaleY + stageScaleY;

			while (colX && colY)
			{
				model_->mat_.trans_.y_ += 1;

				playerTransY = model_->mat_.trans_.y_;

				DisX = playerTransX - stageTransX;
				DisY = playerTransY - stageTransY;

				DisX = abs(DisX);
				DisY = abs(DisY);

				colX = DisX <= playerScaleX + stageScaleX;
				colY = DisY <= playerScaleY + stageScaleY;

				jumpCount = 0;
			}
		}

		return true;
	}

	return false;
}

bool Player::StageCollision(MyMath::ObjMatrix stage)
{
	MoveX();
	bool X = StageCollsionX(stage);

	MoveY();
	bool Y = StageCollsionY(stage);

	return X || Y;
}

bool Player::PlayerCollision(MyMath::ObjMatrix enemy)
{
	Vector3D nowPos = GetPos();
	Vector3D nowScale = GetScale();

	float a = (enemy.trans_.x_ - nowPos.x_) * (enemy.trans_.x_ - nowPos.x_);
	float b = (enemy.trans_.y_ - nowPos.y_) * (enemy.trans_.y_ - nowPos.y_);

	float c = enemy.scale_.x_ * nowScale.x_;

	//あたり判定
	if (a + b < c)
	{
		if (lesFlag_ == 0)
		{
			LesLife();
			knockBackVec_ = nowPos.x_ - enemy.trans_.x_;
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

		model_->mat_.rotAngle_.z_ += spd;

		if (warpMord_ == (uint32_t)Zero)
		{
			if (model_->mat_.trans_.x_ > warpPos_[(uint32_t)Zero].x_ - Ten)
			{
				model_->mat_.trans_.x_ -= distance;
			}
			else
			{
				warpMord_ = (uint32_t)One;
			}
		}
		else if (warpMord_ == (uint32_t)One)
		{
			model_->mat_.scale_ -= {scale, scale, scale};

			if (model_->mat_.trans_.x_ < warpPos_[(uint32_t)Zero].x_)
			{
				model_->mat_.trans_.x_ += distance;
			}
			else
			{
				warpMord_ = (uint32_t)Two;
			}
		}
		else if (warpMord_ == (uint32_t)Two)
		{
			model_->mat_.scale_ = { Zero,Zero,Zero };

			model_->mat_.trans_.x_ += Two;
		}
		else if (warpMord_ == (uint32_t)Three)
		{
			model_->mat_.rotAngle_.z_ = Zero;

			if (model_->mat_.scale_.x_ < Three)
			{
				model_->mat_.scale_ += {scale, scale, scale};
			}
			else
			{
				model_->mat_.scale_ = { Three,Three,Three };
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

	model_->mat_.trans_.x_ += x;
	model_->mat_.trans_.y_ += y;

	if (sheikF_ > 0)
	{
		sheikF_--;
	}
}

void Player::SetCamera(Matrix matView, Matrix matProjection)
{
	//座標Update
	model_->MatUpdate(matView, matProjection);

	for (size_t i = 0; i < attack_.size(); i++)
	{
		attack_[i]->Update(matView, matProjection);

		if (attack_[i]->GetIsDead())
		{
			attack_.erase(attack_.begin() + i);
		}
	}
}