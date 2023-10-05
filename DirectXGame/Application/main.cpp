#include "GameScene.h"
#include "FlameWork.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	//�錾
	std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();

	//�Q�[�����[�v
	gameScene->Run();

	return 0;
}
