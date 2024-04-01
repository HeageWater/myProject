#include "FlameWork.h"
#include "SceneManager.h"
#include "ChengeScene.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "ModelManager.h"
#include "UiManager.h"

void FlameWork::Update()
{
	//下の処理まとめよう
	//Escapeで抜けるかどうかは後で消す
	win_->MsgUpdate();
	if (win_->EndLoop())// || Input::GetInstance()->GetTrigger(DIK_ESCAPE)
	{
		//ポーズクラスを作りインスタンスでフラグを取得し
		SetEndRwqust(true);
	}

	//SetCameraが必要
	//ParticleManager::GetInstance()->Update();

	//キーボードの受付更新
	Input::GetInstance()->Update();

	//当たり判定
	CollisionManager::GetInstance()->Update();

	//
	MyDirectX::GetInstance()->UpdateFPS();
}

void FlameWork::Initialize()
{
	//windowApi
	win_ = std::make_unique<Window>();

	//DirextXの初期化
	MyDirectX::GetInstance()->Initialize(win_.get());

	//ImGuiの初期化
	ImguiManager::GetInstance()->Initialize(win_.get());

	//buff
	cBuff_ = std::make_unique<ConstBuff>(MyDirectX::GetInstance()->GetDev());

	////shader
	//shader_.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	//bilShader_.Initizlize(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");
	//spriteShader_.Initizlize(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");

	////pipeline
	//pipeline_ = std::make_unique<GPipeline>();
	//pipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), shader_);

	////描画初期化
	//multipathPipeline_ = std::make_unique<GPipeline>();
	//multipathPipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), bilShader_);

	////gpipeline
	//uiPipeline_ = std::make_unique<GPipeline>();
	//uiPipeline_->Initialize(MyDirectX::GetInstance()->GetDev(), bilShader_);

	//キーボード
	Input::GetInstance()->Initialize(win_.get());

	//sound
	MyXAudio::GetInstance()->Initialize();

	//仮置き
	Matrix matProjection = MyMath::PerspectiveFovLH(
		Window::window_width_, Window::window_height_,
		MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);

	//音読み込み
	hitSound_ = MyXAudio::GetInstance()->SoundLoadWave("Resources/sound/BGM.wav");
	
	//BGMを鳴らす
	MyXAudio::GetInstance()->SoundPlayLoopWave(hitSound_);

	//テクスチャマネージャ
	TextureManager::GetInstance()->StaticInitialize();

	//欲しい機能
	{
		/*Camera::StaticInitialize(windowsApp_.get());
		
		SpriteCommon::StaticInitialize();
		Sprite2D::StaticInitialize(windowsApp_.get());

		Model::StaticInitialize();

		ParticleManager::StaticInitialize();

		LightManager::StaticInitialize(DirectXBase::GetInstance()->GetDevice().Get())*/
	}

	//シーンチェンジ用クラス
	ChengeScene::GetInstance()->Initialize(matProjection);

	//シーンマネージャー
	SceneFactory* sceneFactory = new SceneCreate();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	//パーティクルマネージャー
	ParticleManager::GetInstance()->Initalize();

	//モデルマネージャー
	ModelManager::GetInstance()->Initialize();

	//UIマネージャー
	UiManager::GetInstance()->Initialize();
}

void FlameWork::Finalize()
{
	ImguiManager::GetInstance()->Finalize();
	Input::GetInstance()->Finalize();
	MyXAudio::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();
}

void FlameWork::Run()
{
	//初期化
	Initialize();

	//ゲームループ
	while (true)
	{
#ifdef _DEBUG

		//ImGui受付開始
		//ImguiManager::GetInstance()->Begin();

#endif _DEBUG

		//更新
		Update();

#ifdef _DEBUG

		//ImGui受付終了
		//ImguiManager::GetInstance()->End();

#endif _DEBUG

		//描画
		Draw();

#ifdef _DEBUG

		//ImGui描画
		//ImguiManager::GetInstance()->Draw();

#endif _DEBUG

		//もしエンドフラグがTrueなら抜ける
		if (IsEndRequst())
		{
			break;
		}
	}

	//終了処理
	Finalize();
}