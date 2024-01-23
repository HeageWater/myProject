#include "BulletEnemy.h"
#include "ENum.h"
#include "DirectX.h"

void BulletEnemy::Initialize(Shader shader, GPipeline* pipeline)
{
	shader_ = shader;
	pipeline_ = pipeline;

	enemy_.Initialize(MyDirectX::GetInstance(), shader_, "Resources\\Model\\enemy\\enemy.obj", pipeline_);

	//mat初期化
	enemy_.mat_.Initialize();

	//スケールをセット
	enemy_.mat_.scale_ = { 1,1,1 };

	//死んでいるかのフラグをoffに
	isDead_ = false;

	//移動値
	move_ = { 0,0,0 };

	//向き
	Vec_ = { 0,0,0 };

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");
}

void BulletEnemy::Draw()
{
	//描画
	enemy_.Draw(tex_);

	//弾描画
	for (size_t i = 0; i < bullets_.size(); i++)
	{
		bullets_[i]->Draw();
	}
}

void BulletEnemy::Update(Matrix matView, Matrix matProjection)
{
	float spd = 0.005f;

	if (fireFlag_)
	{
		spd = 0.03f;
	}

	if (deadVec_)
	{
		spd = 0.01f;
	}

	enemy_.mat_.rotAngle_.y_ += spd;

	if (time_ > 0)
	{
		time_--;

		move_ = { 0,0,0 };
	}

	//
	enemy_.mat_.trans_ -= move_;

	//
	DeadVec();

	//
	EnemyAttack();

	//
	BulletUpdate(matView, matProjection);

	//カメラ位置などをセット
	enemy_.SetCamera(matView, matProjection);

	//更新
	enemy_.Update();
}

bool BulletEnemy::GetIsDead()
{
	return isDead_;
}

Model BulletEnemy::GetModel()
{
	return enemy_;
}

bool BulletEnemy::BoxCollision(Model model)
{
	if (deadVec_ == false)
	{
		float a = (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_) * (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_);
		float b = (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_) * (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_);

		float c = model.mat_.scale_.x_ + enemy_.mat_.scale_.x_ * 100;

		//あたり判定
		if (a + b < c * (float)TWO)
		{
			float spd = TWO;
			Vec_ = model.mat_.trans_ - enemy_.mat_.trans_;
			Vec_ *= spd;
			deadVec_ = true;

			return true;
		}

		a = (-model.mat_.trans_.x_ - enemy_.mat_.trans_.x_) * (-model.mat_.trans_.x_ - enemy_.mat_.trans_.x_);
		b = (-model.mat_.trans_.y_ - enemy_.mat_.trans_.y_) * (-model.mat_.trans_.y_ - enemy_.mat_.trans_.y_);

		c = model.mat_.scale_.x_ + enemy_.mat_.scale_.x_;

		//あたり判定
		if (a + b < c)
		{
			float spd = TWO;
			Vec_ = model.mat_.trans_ - enemy_.mat_.trans_;
			Vec_ *= spd;
			deadVec_ = true;

			return true;
		}
	}

	return false;
}

void BulletEnemy::SertchPlayer(Model model)
{
	playerVec = enemy_.mat_.trans_ - model.mat_.trans_;

	const float sertchScale = 1000;

	float a = (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_) * (model.mat_.trans_.x_ - enemy_.mat_.trans_.x_);
	float b = (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_) * (model.mat_.trans_.y_ - enemy_.mat_.trans_.y_);

	float c = model.mat_.scale_.x_ + sertchScale;

	//あたり判定
	if (a + b < c)
	{
		//攻撃準備
		fireFlag_ = true;
	}
	else
	{
		fireFlag_ = false;
	}
}

void BulletEnemy::DeadVec()
{
	if (deadVec_)
	{
		enemy_.mat_.trans_ -= Vec_;

		bool Y = -15 > enemy_.mat_.trans_.y_ || 1000 < enemy_.mat_.trans_.y_;
		bool X = 0 > enemy_.mat_.trans_.x_ || 1000 < enemy_.mat_.trans_.x_;

		if (Y || X)
		{
			deadVec_ = false;
			isDead_ = true;
		}
	}
}

void BulletEnemy::BulletUpdate(Matrix matView, Matrix matProjection)
{
	//弾更新
	for (size_t i = 0; i < bullets_.size(); i++)
	{
		bullets_[i]->SetCamera(matView, matProjection);
		bullets_[i]->Update();

		if (bullets_[i]->GetIsDead())
		{
			bullets_.erase(bullets_.begin() + i);
		}
	}
}

std::vector<Bullet*> BulletEnemy::GetBullet()
{
	return bullets_;
}

void BulletEnemy::EnemyAttack()
{
	if (!fireFlag_)
	{
		return;
	}

	attackTime_++;

	const size_t AttackTime = 300;

	if (attackTime_ > AttackTime)
	{
		attackTime_ = ZERO;

		//弾生成
		CreateBullet();
	}
}

void  BulletEnemy::CreateBullet()
{
	//モデルを指定して3Dオブジェクトを生成
	Bullet* newBullet_ = new Bullet();

	//初期化
	newBullet_->Initialize(shader_, pipeline_);

	//trans
	newBullet_->SetPos(enemy_.mat_.trans_);

	//Vec
	newBullet_->SetVec(playerVec.normalize());

	//格納
	bullets_.push_back(newBullet_);
}