#include "FlameWork.h"

void FlameWork::Update()
{
	/*win->MsgUpdate();
	if (win->EndLoop())
	{
		SetEndRwqust(true);
	}*/
}

void FlameWork::Initilize()
{
	////windowApi
	//win = std::make_unique<Window>();

	////dxCommon
	//dx = std::make_unique<MyDirectX>(win.get());

	////buff
	//cBuff = std::make_unique<ConstBuff>(dx->GetDev(), (float)win->window_width, (float)win->window_height);

	////pipeline
	//pipeline = std::make_unique<GPipeline>();
	//pipeline->Initialize(dx->GetDev(), shader);

	////�`�揉����
	//multipathPipeline = std::make_unique<GPipeline>();
	//multipathPipeline->Initialize(dx->GetDev(), bilShader);

	////screen
	//screen.Initialize(dx.get(), multipathPipeline.get(), bilShader);
	//screen.obj.trans.z = 100.1f;
	//screen.obj.scale = { Window::window_width * 2,Window::window_height / 2,0.2f };

	////gpipeline
	//uiPipeline = std::make_unique<GPipeline>();
	//uiPipeline->Initialize(dx->GetDev(), bilShader);
}

void FlameWork::Finalize()
{
}

void FlameWork::Run()
{
	////������
	//Initilize();

	////�Q�[�����[�v
	//while (true)
	//{
	//	//�X�V
	//	Update();

	//	//�`��
	//	Draw();

	//	//�����G���h�t���O��True�Ȃ甲����
	//	if (IsEndRequst() == true)
	//	{
	//		break;
	//	}
	//}

	////�I������
	//Finalize();
}