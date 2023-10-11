#include "FlameWork.h"
#include "SceneManager.h"
#include "ChengeScene.h"

void FlameWork::Update()
{
	//下の処理まとめよう
	//Escapeで抜けるかどうかは後で消す
	win->MsgUpdate();
	if (win->EndLoop() || Input::GetInstance()->GetTrigger(DIK_ESCAPE))
	{
		SetEndRwqust(true);
	}

	Input::GetInstance()->Update();
}

void FlameWork::Initialize()
{
	//windowApi
	win = std::make_unique<Window>();

	//DirextXの初期化
	MyDirectX::GetInstance()->Initialize(win.get());

	//ImGuiの初期化
	ImguiManager::GetInstance()->Initialize();

	//buff
	cBuff = std::make_unique<ConstBuff>(MyDirectX::GetInstance()->GetDev());

	//shader
	shader.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	bilShader.Initizlize(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");
	spriteShader.Initizlize(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");

	//pipeline
	pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(MyDirectX::GetInstance()->GetDev(), shader);

	//描画初期化
	multipathPipeline = std::make_unique<GPipeline>();
	multipathPipeline->Initialize(MyDirectX::GetInstance()->GetDev(), bilShader);

	//gpipeline
	uiPipeline = std::make_unique<GPipeline>();
	uiPipeline->Initialize(MyDirectX::GetInstance()->GetDev(), bilShader);

	//キーボード
	Input::GetInstance()->Initialize(win.get());

	//sound
	MyXAudio::GetInstance()->Initialize();

	//仮置き
	Matrix matProjection = MyMath::PerspectiveFovLH(
		Window::window_width, Window::window_height,
		MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);

	{
		/*Camera::StaticInitialize(windowsApp_.get());

		TextureManager::GetInstance()->StaticInitialize();

		SpriteCommon::StaticInitialize();
		Sprite2D::StaticInitialize(windowsApp_.get());

		Model::StaticInitialize();

		ParticleManager::StaticInitialize();

		LightManager::StaticInitialize(DirectXBase::GetInstance()->GetDevice().Get())*/
	}

	//シーンチェンジ用クラス
	ChengeScene::GetInstance()->Initialize(matProjection);

	//
	SceneFactory* sceneFactory = new SceneCreate();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
}

void FlameWork::Finalize()
{
	ImguiManager::GetInstance()->Finalize();
	Input::GetInstance()->Finalize();
	MyXAudio::GetInstance()->Finalize();
}

void FlameWork::Run()
{
	//初期化
	Initialize();

	//ゲームループ
	while (true)
	{
		////ImGui受付開始
		ImguiManager::GetInstance()->Begin();

		//更新
		Update();

		////ImGui受付終了
		ImguiManager::GetInstance()->End();

		//描画
		Draw();

#ifdef _DEBUG

		ImguiManager::GetInstance()->Draw();

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