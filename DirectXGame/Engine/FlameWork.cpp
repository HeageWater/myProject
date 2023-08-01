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

	//dxCommon
	dx = std::make_unique<MyDirectX>(win.get());

	//buff
	cBuff = std::make_unique<ConstBuff>(dx->GetDev(), (float)win->window_width, (float)win->window_height);

	//shader
	shader.Initizlize(L"Resources/shader/BasicVS.hlsl", L"Resources/shader/BasicPS.hlsl");
	bilShader.Initizlize(L"Resources/shader/VShader.hlsl", L"Resources/shader/PShader.hlsl");
	spriteShader.Initizlize(L"Resources/shader/SpriteVS.hlsl", L"Resources/shader/SpritePS.hlsl");

	//pipeline
	pipeline = std::make_unique<GPipeline>();
	pipeline->Initialize(dx->GetDev(), shader);

	//描画初期化
	multipathPipeline = std::make_unique<GPipeline>();
	multipathPipeline->Initialize(dx->GetDev(), bilShader);

	//gpipeline
	uiPipeline = std::make_unique<GPipeline>();
	uiPipeline->Initialize(dx->GetDev(), bilShader);
}

void FlameWork::Finalize()
{
}

void FlameWork::Run()
{
	//初期化
	Initilize();

	//ゲームループ
	while (true)
	{
		//更新
		Update();

		//描画
		Draw();

		//もしエンドフラグがTrueなら抜ける
		if (IsEndRequst() == true)
		{
			break;
		}
	}

	//終了処理
	Finalize();
}