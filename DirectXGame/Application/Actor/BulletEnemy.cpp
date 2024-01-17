#include "BulletEnemy.h"
#include "ENum.h"
#include "DirectX.h"

void BulletEnemy::Initialize(Shader shader, GPipeline* pipeline_)
{
	enemy_.Initialize(MyDirectX::GetInstance(), shader, "Resources\\Model\\enemy\\enemy.obj", pipeline_);

	//mat初期化
	enemy_.mat_.Initialize();

	//スケールをセット
	enemy_.mat_.scale_ = { 1,1,1 };

	//
	isDead_ = false;

	//
	move_ = { 0,0,0 };

	//
	Vec_ = { 0,0,0 };

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");
}

void BulletEnemy::Draw()
{
	//描画
	enemy_.Draw(tex_);
}

void BulletEnemy::Update(Matrix matView, Matrix matProjection)
{
	float spd = 0.005f;

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
	//BulletUpdate(matView, matProjection);

	//カメラ位置などをセット
	enemy_.SetCamera(matView,matProjection);

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
	Matrix mat = matView;
	Matrix mat2 = matProjection;
	mat = mat2;

	//弾更新
	for (size_t i = 0; i < bullets_.size(); i++)
	{
		/*bullets_[i]->Update(view_, prodaction_);

		if (bullets_[i]->GetIsDead())
		{
			bullets_.erase(bullets_.begin() + i);
		}*/
	}

	//弾描画
	for (size_t i = 0; i < bullets_.size(); i++)
	{
		//bullets_[i]->Draw(enemyTex_);
		//bullets_[i]->Draw();
	}

	//今ある弾を削除
	for (size_t i = 0; i < bullets_.size(); i++)
	{
		//bullets_.erase(bullets_.begin());
	}

	//Create() {
	//	//モデルを指定して3Dオブジェクトを生成
	//			//Enemy* newModel_ = new Enemy();
	//	BulletEnemy* newModel_ = new BulletEnemy();
	//	newModel_->Initialize(shader_, pipeline_);

	//	//trans
	//	newModel_->SetTrans(objectdata.translation_ * pos);

	//	//rotation
	//	newModel_->SetRot(objectdata.rotation_);

	//	//scale;
	//	newModel_->SetScale(objectdata.scaling_ * enemyScale);

	//	//Update
	//	newModel_->Update(view_, prodaction_);

	//	//格納
	//	enemies_.push_back(newModel_);
	//}

}

std::vector<Bullet*> BulletEnemy::GetBullet()
{
	return bullets_;
}

void BulletEnemy::EnemyAttack()
{
	if (fireFlag_)
	{

	}
}