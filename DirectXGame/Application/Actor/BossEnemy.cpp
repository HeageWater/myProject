#include "BossEnemy.h"
#include "CollisionManager.h"

void BossEnemy::Initialize(Shader shader, GPipeline* pipeline)
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

	//mat初期化
	model_->mat_.Initialize();

	//スケールをセット
	model_->mat_.scale_ = { 3,3,3 };

	//死んでいるかのフラグをoffに
	isDead_ = false;

	//移動値
	move_ = { 0,0,0 };

	//向き
	vec_ = { 0,0,0 };

	//画像読み込み
	tex_ = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/br.png");

	//ヒットポイントセット
	hitpoint = 20;
}

void BossEnemy::Draw()
{
	//描画
	model_->Draw(tex_);
}

void BossEnemy::Update(Matrix matView, Matrix matProjection)
{



	//カメラ位置などをセット
	model_->SetCamera(matView, matProjection);

	//更新
	model_->Update();
}

const bool BossEnemy::GetIsDead()
{
	return isDead_;
}
