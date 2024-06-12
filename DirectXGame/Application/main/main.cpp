#include "MyGame.h"
#include "FlameWork.h"
//#include "GameScene.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	//宣言
	std::unique_ptr<FlameWork> gameScene = std::make_unique<MyGame>();

	//ゲームループ
	gameScene->Run();

	return 0;
}
