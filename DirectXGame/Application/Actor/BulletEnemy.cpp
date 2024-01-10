#include "BulletEnemy.h"
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
}

void BulletEnemy::Draw()
{
	//描画
	enemy_.Draw(tex_);
}

void BulletEnemy::Update(Matrix matView, Matrix matProjection)
{


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
