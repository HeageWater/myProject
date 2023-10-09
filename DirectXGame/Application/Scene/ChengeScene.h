#pragma once
#include "Sprite.h"

class ChengeScene
{
public:
	ChengeScene();
	~ChengeScene();
	void Initialize(Matrix matProjection);
	void Draw(size_t tex);
	void Draw();
	void Update();
	void Reset();

	void SetPlayFlag();
	bool GetPlayFlag() { return isPlayFlag; };

	float GetTime() { return time; };
private:

	//これをONにするとシーンチェンジが開始する
	bool isPlayFlag = false;

	//sprite
	//SpriteCommon* spriteCommon = SpriteCommon::Get();
	SpriteCommon* spriteCommon = new SpriteCommon();
	Sprite* sprite_ = new Sprite();

	size_t tex;

	float time;
};