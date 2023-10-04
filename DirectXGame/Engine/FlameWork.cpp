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

	//DirextX�̏�����
	MyDirectX::GetInstance()->Initialize(win.get());

	//ImGui�̏�����
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

	//�`�揉����
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
	//������
	Initilize();

	//�Q�[�����[�v
	while (true)
	{
		////ImGui��t�J�n
		//ImguiManager::GetInstance()->Begin();

		//�X�V
		Update();

		////ImGui��t�I��
		//ImguiManager::GetInstance()->End();

		//�`��
		Draw();

		/*ImguiManager::GetInstance()->Draw();*/

		//�����G���h�t���O��True�Ȃ甲����
		if (IsEndRequst())
		{
			break;
		}
	}

	//�I������
	Finalize();
}