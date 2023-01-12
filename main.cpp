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
#include <random>
#include "Re//Model.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Timer.h"
//#include "RE//AudioManager.h"
//#include "Sound.h"

enum Scene {
	Title,
	Play,
	Clear,
	Over,
};

int GetRandomA(int min, int max)
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<> distr(min, max);
	return distr(eng);
}

float Sheiku(int count) {
	float sheiku;
	int rand = GetRandomA(1, 3);
	if (count % 2 == 0)
		rand = -rand;
	sheiku = rand;
	return sheiku;
}

bool c(Vector2D player, Vector2D stage, float ab, float bc)
{
	if (player.x - (ab) <= stage.x + (bc) &&
		player.x + (ab) >= stage.x - (bc) &&
		player.y - (ab) <= stage.y + (bc) &&
		player.y + (ab) >= stage.y - (bc)
		)
	{
		return true;
	}

	return false;
}

bool CircleCollsionEnemy(Vector2D enemy1, Vector2D enemy2, float ab, float bc) {

	int a = (enemy1.x - enemy2.x) * (enemy1.x - enemy2.x);
	int b = (enemy1.y - enemy2.y) * (enemy1.y - enemy2.y);

	int c = ab * bc;

	//あたり判定
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

	int numG[10];
	numG[0] = dx->LoadTextureGraph(L"Resources/number/num0.png");
	numG[1] = dx->LoadTextureGraph(L"Resources/number/num1.png");
	numG[2] = dx->LoadTextureGraph(L"Resources/number/num2.png");
	numG[3] = dx->LoadTextureGraph(L"Resources/number/num3.png");
	numG[4] = dx->LoadTextureGraph(L"Resources/number/num4.png");
	numG[5] = dx->LoadTextureGraph(L"Resources/number/num5.png");
	numG[6] = dx->LoadTextureGraph(L"Resources/number/num6.png");
	numG[7] = dx->LoadTextureGraph(L"Resources/number/num7.png");
	numG[8] = dx->LoadTextureGraph(L"Resources/number/num8.png");
	numG[9] = dx->LoadTextureGraph(L"Resources/number/num9.png");

	int bgPng = dx->LoadTextureGraph(L"Resources/title.png");
	int plPng = dx->LoadTextureGraph(L"Resources/pless.png");
	int brPng = dx->LoadTextureGraph(L"Resources/br.png");

	//AudioManager* audioManager = nullptr;
	//audioManager = audioManager->GetInstance();

	//uint32_t BGM = audioManager->LoadAudio("Resources/sound/BGM.wav");

	MyDebugCamera debugcamera(Vector3D(0.0f, 30.0f, 100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

	std::unique_ptr<ConstBuff> cBuff(new ConstBuff(dx->GetDev(), win->window_width, win->window_height));

	std::unique_ptr<Input> input(new Input(win.get()));

	Shader shader(L"BasicVS.hlsl", L"BasicPS.hlsl");
	Shader bilShader(L"VShader.hlsl", L"PShader.hlsl");
	Shader spriteShader(L"SpriteVS.hlsl", L"SpritePS.hlsl");

	std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));

	//描画初期化
	std::unique_ptr<GPipeline> multipathPipeline(new GPipeline(dx->GetDev(), bilShader));

	Square screen(dx.get(), multipathPipeline.get(), bilShader);
	screen.obj.trans.z = 0.1f;
	screen.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	std::unique_ptr<GPipeline> uiPipeline(new GPipeline(dx->GetDev(), bilShader));

	Square title(dx.get(), uiPipeline.get(), spriteShader);
	title.obj.scale = { 640,320,0.2f };

	Square pressText(dx.get(), uiPipeline.get(), bilShader);
	pressText.obj.trans.y = -200;
	pressText.obj.scale = { 120,120,0.2f };


	Square scoreText(dx.get(), multipathPipeline.get(), bilShader);
	scoreText.obj.trans.x = -360;
	scoreText.obj.scale = { 160,90,0.2f };

	Square backGround(dx.get(), multipathPipeline.get(), spriteShader);
	backGround.obj.trans.z = 0.1f;
	backGround.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	Square bGround(dx.get(), multipathPipeline.get(), spriteShader);
	bGround.obj.trans.z = 0.1f;
	bGround.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	Matrix spriteProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.0f, 1.0f);
	bGround.MatUpdate(Matrix(), spriteProjection, 0);
	pressText.MatUpdate(Matrix(), spriteProjection, 0);

	Timer timer(dx.get(), uiPipeline.get(), &bilShader, numG);

	Square goal(dx.get(), multipathPipeline.get(), spriteShader);
	goal.obj.trans.x = Window::window_width;
	goal.obj.scale = { Window::window_width / 2,Window::window_height / 2,0.2f };

	//描画用行列
	MyMath::MatView matView;
	matView.Init(Vector3D(0.0f, 0.0f, -100.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
	Matrix matProjection = MyMath::PerspectiveFovLH(Window::window_width, Window::window_height, MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
	Matrix orthoProjection = MyMath::OrthoLH(Window::window_width, Window::window_height, 0.1f, 1000.0f);

	//Initialize
	//std::unique_ptr<GPipeline> pipeline(new GPipeline(dx->GetDev(), shader));
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
	player.player.mat.scale = { 5,5,5 };
	player.player.mat.rotAngle = { 0,0,0 };

	/*Enemy enemy;
	enemy.Initialize(dx.get(), shader, pipeline.get());

	enemy.enemy.mat.trans = { 5,0,0 };
	enemy.enemy.mat.scale = { 5,5,5 };
	enemy.enemy.mat.rotAngle = { 0,0,0 };*/

	int scene = Title;

	int sheikuCount = 0;
	bool sheikuFlag = false;

	//audioManager->PlayWave(BGM,true);

	std::vector<Enemy*> enemy;
	int a = 0;
	float r = 0;
	int time = 0;

	//	ゲームループ
	while (true)
	{
		//WinMsg
		win->MsgUpdate();
		if (win->EndLoop()) { break; }

		//Update
		input->Update();

		debugcamera.Update(*input);

		screen.MatUpdate(matView.mat, orthoProjection, 0);

		//ここまで

		if (input->GetTrigger(DIK_E)) {
			if (sheikuFlag == false) {
				sheikuFlag = true;
				sheikuCount = 10;
			}
		}

		switch (scene)
		{

		case Title:

			if (input->GetTrigger(DIK_SPACE)) {
				scene = Play;
			}

			r = sin(3.1415f * 2 / 240 * time);
			bGround.obj.trans = { 0,r * 150,0 };

			bGround.MatUpdate(Matrix(), spriteProjection, 0);

			break;

		case Play:

			time++;

			//ステージとプレイヤーの当たり判定
		//if()
		/*for (int i = 0; i < stage.model.size(); i++)
		{
			if (false) {
				player.player.mat.trans.x = max(player.player.mat.trans.x, stage.model[i]->mat.trans.x);
				player.player.mat.trans.x = min(player.player.mat.trans.x, stage.model[i]->mat.trans.x);

				player.player.mat.trans.y = max(player.player.mat.trans.y, stage.model[i]->mat.trans.y);
				player.player.mat.trans.y = min(player.player.mat.trans.y, stage.model[i]->mat.trans.y);
			}
		}*/
		//debugcamera.eye.x += 1.0f * (input->GetKey(DIK_I) - input->GetKey(DIK_O));
		//debugcamera.target.x += 1.0f * (input->GetKey(DIK_I) - input->GetKey(DIK_O));

			a = GetRandomA(1, 8);

			if (enemy.size() < 3) {
				if (a > 6) {
					//仮沖の中身を作る
					Enemy* newenemy = new Enemy();

					float size = 10;

					newenemy->Initialize(dx.get(), shader, pipeline.get());

					newenemy->enemy.mat.trans = { 0,enemy.size() * size,0 };

					newenemy->enemy.mat.scale = { 3,3,3 };

					//格納
					enemy.push_back(newenemy);
				}
			}


			if (sheikuFlag == true) {
				debugcamera.Move(Sheiku(sheikuCount));
				sheikuCount--;

				if (sheikuCount < 0) {
					sheikuFlag = false;
					debugcamera.eye = { 0.0f, 30.0f, 100.0f };
				}
			}

			//debugcamera.eye.z += 1.0f * (input->GetKey(DIK_I) - input->GetKey(DIK_O));

			//debugcamera.MatUpdate();

			//box.mat.trans.x += 1.0f * (input->GetKey(DIK_D) - input->GetKey(DIK_A));
			//box.mat.rotAngle.x +=  0.1f * (input->GetKey(DIK_D) - input->GetKey(DIK_A));

			//box2.mat.rotAngle.x += 0.1f * (input->GetKey(DIK_W) - input->GetKey(DIK_S));

			//box.MatUpdate(debugcamera.mat, matProjection);
			//box2.MatUpdate(debugcamera.mat, matProjection);

			player.Update(debugcamera.mat, matProjection);

			for (int i = 0; i < enemy.size(); i++)
			{
				enemy[i]->Update(debugcamera.mat, matProjection);
			}

			stage.Update(debugcamera.mat, matProjection);

			if (input->GetTrigger(DIK_SPACE)) {
				scene = Clear;
			}

			if (input->GetTrigger(DIK_0)) {
				scene = Over;
			}

			break;

		case Over:
			if (input->GetTrigger(DIK_SPACE)) {
				scene = Title;
			}
			break;

		case Clear:
			if (input->GetTrigger(DIK_SPACE)) {
				scene = Title;
			}
			break;

		default:
			break;
		}



		////box.mat.trans += move;

		//debugcamera.eye.z += 1.0f * (input->GetKey(DIK_I) - input->GetKey(DIK_O));

		////debugcamera.MatUpdate();

		//box.mat.trans.x += 1.0f * (input->GetKey(DIK_D) - input->GetKey(DIK_A));
		////box.mat.rotAngle.x +=  0.1f * (input->GetKey(DIK_D) - input->GetKey(DIK_A));

		//box2.mat.rotAngle.x += 0.1f * (input->GetKey(DIK_W) - input->GetKey(DIK_S));

		//box.MatUpdate(debugcamera.mat, matProjection);
		//box2.MatUpdate(debugcamera.mat, matProjection);

		//player.Update(debugcamera.mat, matProjection);
		//enemy.Update(debugcamera.mat, matProjection);

		//Draw
		dx->PrevDrawScreen();

		// 描画コマンド

		switch (scene)
		{

		case Title:
			//box2.Draw(white);

			bGround.Draw(bgPng);

			pressText.Draw(plPng);
			break;

		case Play:
			stage.Draw(brPng);

			for (int i = 0; i < enemy.size(); i++)
			{
				/*if(BoxCollision(Vector2D(player.attackModel.mat.trans.x, player.attackModel.mat.trans.y),
					Vector2D(enemy[i]->enemy.mat.trans.x, player.attackModel.mat.trans.y), ,
					))*/

				if (CircleCollsionEnemy(Vector2D(player.attackModel.mat.trans.x, player.attackModel.mat.trans.y),
					Vector2D(enemy[i]->enemy.mat.trans.x, enemy[i]->enemy.mat.trans.y),
					player.attackModel.mat.scale.x,
					enemy[i]->enemy.mat.scale.x)) {

					if (enemy[i]->hitF != true) {
						enemy[i]->trans = player.attackModel.mat.trans - enemy[i]->enemy.mat.trans;
						enemy[i]->trans.normalize();
						enemy[i]->hitF = true;

						if (sheikuFlag == false) {
							sheikuFlag = true;
							sheikuCount = 10;
						}
					}
				}
			}

			player.Draw(texP);

			for (int i = 0; i < enemy.size(); i++)
			{
				enemy[i]->Draw(white, debugcamera.mat, matProjection);
			}

			for (int i = 0; i < enemy.size(); i++)
			{
				if (enemy[i]->enemy.mat.trans.x > 50 || enemy[i]->enemy.mat.trans.y > 50) {
					enemy.erase(enemy.begin());
				}
			}

			break;

		case Over:
			break;

		case Clear:
			break;

		default:
			break;
		}

		dx->PostDrawScreen();

		//UIDraw
		dx->PrevDraw();

		screen.Draw(0);

		dx->PostDraw();
	}
	return 0;
}