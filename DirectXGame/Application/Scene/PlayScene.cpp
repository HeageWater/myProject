#include "PlayScene.h"
#include "imgui.h"
#include "ChengeScene.h"

void PlayScene::Update()
{
	//ImGui受付開始
	//ImGui::Begin("player Pos");

	//float a = player->GetPos().x;

	//ImGui::SliderFloat("player pos", &a, -400, 400);

	////ImGui受付終了
	//ImGui::End();

	//player更新
	player->Update(matView.mat, matProjection, shader);

	//カメラ更新
	matView.MatUpdate();

	//スクリーン更新
	screen.MatUpdate(matView.mat, matProjection, 0);

	//シーンチェンジテスト
	if (input_->GetTrigger(DIK_SPACE))
	{
		ChengeScene::GetInstance()->SetPlayFlag("TITLE");
	}

	//シーンチェンジ更新
	ChengeScene::GetInstance()->Update();
}

void PlayScene::Initialize()
{
	//描画用行列
	matView.Init(Vector3D(0.0f, 60.0f, -50.0f), Vector3D(0.0f, 30.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	//箱画像
	white = MyDirectX::GetInstance()->LoadTextureGraph(L"Resources/sprite/cube.jpg");

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
}

void PlayScene::Draw()
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

	//シーンチェンジ描画
	ChengeScene::GetInstance()->Draw();

	//描画受付終了
	MyDirectX::GetInstance()->PostDraw();
}

void PlayScene::Finalize()
{
}
