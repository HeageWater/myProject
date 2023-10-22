#include "TitleScene.h"
#include "imgui.h"
#include "ChengeScene.h"

void TitleScene::Update()
{
	//player更新
	player->Update(matView.mat, matProjection, shader);

	Vector3D pos = { (float)(input_->GetKey(DIK_D) - input_->GetKey(DIK_A)),(float)(input_->GetKey(DIK_S) - input_->GetKey(DIK_W)),0 };
	pos += player->GetPos();
	player->SetPos(pos);

	//targetをplayerに
	matView.eye.x = player->GetPos().x;
	matView.target.x = player->GetPos().x;

	matView.eye.y = player->GetPos().y;
	matView.target.y = player->GetPos().y;

	//jsonファイルから読み込んだものの更新
	LoadObjectData::GetInstance()->SetCamera(matView.mat, matProjection);
	LoadObjectData::GetInstance()->Update();
	//カメラ更新
	matView.MatUpdate();

	//スクリーン更新
	screen.MatUpdate(matView.mat, matProjection, 0);

	//シーンチェンジテスト
	if (input_->GetTrigger(DIK_SPACE))
	{
		ChengeScene::GetInstance()->SetPlayFlag("PLAY");
	}

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void TitleScene::Initialize()
{
	//描画用行列
	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//白画像
	white = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/background.png");

	//shader
	shader.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	bilShader.Initizlize(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");

	//pipeline
	pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(MyDirectX::GetInstance()->GetDev(), shader);

	//描画初期化
	multipathPipeline = std::make_unique<GPipeline>();
	multipathPipeline->Initialize(MyDirectX::GetInstance()->GetDev(), bilShader);

	//背景のスクリーン(これが必要なので依存しないようにしたい)
	screen.Initialize(multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 100.1f;
	screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };

	//player
	player->Initialize(shader, pipeline.get());

	//jsonファイルから読み込んだものの初期化
	LoadObjectData::GetInstance()->SetModel(shader, pipeline.get());
	LoadObjectData::GetInstance()->Initialize();
}

void TitleScene::Draw()
{
	//Draw
	MyDirectX::GetInstance()->PrevDrawScreen();

	//// 描画コマンド
	MyDirectX::GetInstance()->PostDrawScreen();

	//UIDraw
	MyDirectX::GetInstance()->PrevDraw();

	//スクリーン描画
	screen.Draw(white);

	//Actor描画
	player->Draw(white, white);

	//jsonファイルから読み込んだものの描画
	LoadObjectData::GetInstance()->Draw();

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void TitleScene::Finalize()
{
}