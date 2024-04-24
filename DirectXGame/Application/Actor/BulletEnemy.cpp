#include "BulletEnemy.h"
#include "ENum.h"
#include "DirectX.h"
#include "CollisionManager.h"

void BulletEnemy::Initialize(Shader shader, GPipeline* pipeline)
{
	//modelを制作
	model_ = std::make_unique<Model>();

	//タグ
	tag_ = "enemy";
	CollisionManager::GetInstance()->AddCollision(this);

	shader_ = shader;
	pipeline_ = pipeline;

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("Resources\\Model\\enemy\\enemy.obj");

	//モデルセット
	model_ = SetModel("Resources\\Model\\enemy\\enemy.obj");

	//model_->Initialize(MyDirectX::GetInstance(), shader_, "Resources\\Model\\enemy\\enemy.obj", pipeline_);

	//mat初期化
	model_->mat_.Initialize();

	//スケールをセット
	model_->mat_.scale_ = { 1,1,1 };

	//死んでいるかのフラグをoffに
	isDead_ = false;

	//移動値
	move_ = { 0,0,0 };

	//向き
	vec_ = { 0,0,0 };

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");
}

void BulletEnemy::Draw()
{
	//描画
	model_->Draw(tex_);

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
		spd = 0.1f;
	}

	if (deadVec_)
	{
		spd = 0.5f;
	}

	model_->mat_.rotAngle_.y_ += spd;

	if (time_ > 0)
	{
		time_--;

		move_ = { 0,0,0 };
	}

	//
	model_->mat_.trans_ -= move_;

	//
	DeadVec();

	//
	EnemyAttack();

	//
	BulletUpdate(matView, matProjection);

	//カメラ位置などをセット
	model_->SetCamera(matView, matProjection);

	//更新
	model_->Update();
}

const bool BulletEnemy::GetIsDead()
{
	return isDead_;
}

bool BulletEnemy::BoxCollision(MyMath::ObjMatrix model)
{
	if (deadVec_ == false)
	{
		//scale
		float Scale = (model_->mat_.scale_.x_ + model.scale_.x_);

		//判定
		float Dist = (model_->mat_.trans_ - model.trans_).length();

		//あたり判定
		if (Dist <= Scale)
		{
			float spd = FIVE;
			vec_ = model.trans_ - model_->mat_.trans_;
			vec_.normalize();
			vec_ *= spd;
			deadVec_ = true;

			return true;
		}
	}

	return false;
}

void BulletEnemy::SertchPlayer(MyMath::ObjMatrix model)
{
	playerVec = model_->mat_.trans_ - model.trans_;

	const float sertchScale = 7500;

	float a = (model.trans_.x_ - model_->mat_.trans_.x_) * (model.trans_.x_ - model_->mat_.trans_.x_);
	float b = (model.trans_.y_ - model_->mat_.trans_.y_) * (model.trans_.y_ - model_->mat_.trans_.y_);

	float c = model.scale_.x_ + sertchScale;

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
		model_->mat_.trans_ -= vec_;

		bool Y = -15 > model_->mat_.trans_.y_ || 1000 < model_->mat_.trans_.y_;
		bool X = 0 > model_->mat_.trans_.x_ || 1000 < model_->mat_.trans_.x_;

		if (Y || X)
		{
			deadVec_ = false;
			isDead_ = true;
			Destroy();
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

	const size_t AttackTime = 100;

	if (attackTime_ > AttackTime)
	{
		attackTime_ = -200;

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
	newBullet_->SetPos(model_->mat_.trans_);

	//Vec
	newBullet_->SetVec(playerVec.normalize());

	//格納
	bullets_.push_back(newBullet_);
}