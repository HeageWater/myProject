#include "FlameWork.h"

void FlameWork::Update()
{
	win->MsgUpdate();
	if (win->EndLoop())
	{
		SetEndRwqust(true);
	}
}

void FlameWork::Initilize()
{
	//windowApi
	win = std::make_unique<Window>();

	//DirextXの初期化
	MyDirectX::GetInstance()->Initialize(win.get());

	//ImGuiの初期化
	ImguiManager::GetInstance()->Initialize();

	//buff
	cBuff = std::make_unique<ConstBuff>(MyDirectX::GetInstance()->GetDev(), (float)win->window_width, (float)win->window_height);

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
}

void FlameWork::Finalize()
{
	ImguiManager::GetInstance()->Finalize();
}

void FlameWork::Run()
{
	//初期化
	Initilize();

	//ゲームループ
	while (true)
	{
		////ImGui受付開始
		//ImguiManager::GetInstance()->Begin();

		//更新
		Update();

		////ImGui受付終了
		//ImguiManager::GetInstance()->End();

		//描画
		Draw();

		/*ImguiManager::GetInstance()->Draw();*/

		//もしエンドフラグがTrueなら抜ける
		if (IsEndRequst())
		{
			break;
		}
	}

	//終了処理
	Finalize();
}