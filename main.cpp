#include "Re//Window.h"
#include "Re//DirectX.h"
#include "Re//Input.h"
#include "Re//GPipeline.h"
#include "Re//Object3D.h"
#include "Re//ConstBuff.h"
#include "Re//TextureData.h"
#include "Re//Shader.h"
#include "Re//MyDebugCamera.h"
#include "Re//Square.h"
#include "Re//Controller.h"
#include <memory>
#include "Re//Model.h"
#include "Stage.h"
#include "Player.h"

bool CircleCollsionEnemy(Vector2D enemy1, Vector2D enemy2, float ab, float bc) {

	int a = (enemy1.x - enemy2.x) * (enemy1.x - enemy2.x);
	int b = (enemy1.y - enemy2.y) * (enemy1.y - enemy2.y);

	int c = ab * bc;

	//�����蔻��
	if (a + b < c)
	{
		return true;
	}

	return false;
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
#pragma region Initialize
	std::unique_ptr<Window> win(new Window());

	std::unique_ptr<MyDirectX> dx(new MyDirectX(win.get()));
	int texA = dx->LoadTextureGraph(L"Resources/maru/maru.png");
	int white = dx->LoadTextureGraph(L"Resources/white1x1.png");
	int texP = dx->LoadTextureGraph(L"Resources/cube.jpg");

	MyDebugCamera debugcamera(Vector3D(0.0f, 10.0f, 30.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	std::unique_ptr<Input> input(new Input(win.get()));
	//Controller* controller = nullptr;
	//controller = Controller::GetInstance();

	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");

	//�`�揉����
	std::unique_ptr<GPipeline> multipathPipeline(new GPipeline(dx->GetDev(), bilShader));
	Square screen(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 0.1f;
	screen.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	//�`��p�s��
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//Initialize
	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));
	Model box(dx.get(), shader, "Resources\\maru\\maru.obj", pipeline.get());
	Model box2(dx.get(), shader, "Resources\\rasu\\rasu.obj", pipeline.get());

	box.mat.trans = { 0,5,0 };
	box.mat.scale = { 1,1,1 };
	box.mat.rotAngle = { 0,0,0 };
	box.MatUpdate(debugcamera.mat, matProjection);

	box2.mat.trans = { 5,0,0 };
	box2.mat.scale = { 3,3,3 };
	box2.mat.rotAngle = { 0,0,0 };
	box2.MatUpdate(debugcamera.mat, matProjection);

	Stage stage;
	stage.Initialize(dx.get(), shader, pipeline.get());

	Player player;
	player.Initialize(dx.get(), shader, pipeline.get());

	player.player.mat.trans = { 5,0,0 };
	player.player.mat.scale = { 1,1,1 };
	player.player.mat.rotAngle = { 0,0,0 };

	//	�Q�[�����[�v
	while (true)
	{
		//WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }

		//Update
		input->Update();

		//controller->Update();

		debugcamera.Update(*input);

		screen.MatUpdate(matView.mat, orthoProjection);

		stage.Update(debugcamera.mat, matProjection);

		//�����܂�

		//box.mat.trans.y -= 0.01f;

		//�X�e�[�W�ƃv���C���[�̓����蔻��
		//if()
		for (int i = 0; i < stage.model.size(); i++)
		{
			if (false) {
				player.player.mat.trans.x = max(player.player.mat.trans.x, stage.model[i]->mat.trans.x);
				player.player.mat.trans.x = min(player.player.mat.trans.x, stage.model[i]->mat.trans.x);

				player.player.mat.trans.y = max(player.player.mat.trans.y, stage.model[i]->mat.trans.y);
				player.player.mat.trans.y = min(player.player.mat.trans.y, stage.model[i]->mat.trans.y);
			}
		}


		Vector3D move = { 0.0f,0.0f,0.0f };

		/*if (controller->ButtonTriggerPush(A)) {
			move.x = 5.0f;
		}*/

		box.mat.trans += move;

		debugcamera.eye.x += 1.0f * (input->GetKey(DIK_I) - input->GetKey(DIK_O));

		//debugcamera.MatUpdate();

		box.mat.trans.x += 1.0f * (input->GetKey(DIK_D) - input->GetKey(DIK_A));
		//box.mat.rotAngle.x +=  0.1f * (input->GetKey(DIK_D) - input->GetKey(DIK_A));

		box2.mat.rotAngle.x += 0.1f * (input->GetKey(DIK_W) - input->GetKey(DIK_S));

		box.MatUpdate(debugcamera.mat, matProjection);
		box2.MatUpdate(debugcamera.mat, matProjection);

		player.Update(debugcamera.mat, matProjection);

		//Draw
		dx->PrevDrawScreen();

		// �`��R�}���h

		stage.Draw(white);

		if (CircleCollsionEnemy(Vector2D(player.attackModel.mat.trans.x, player.attackModel.mat.trans.y),
			Vector2D(box.mat.trans.x, box.mat.trans.y),
			player.attackModel.mat.scale.x,
			box.mat.scale.x)) {

		}
		else {

			box.Draw(texA);
		}

		player.Draw(texP);

		//box2.Draw(white);

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		screen.Draw(0);

		dx->PostDraw();
	}
	return 0;
}