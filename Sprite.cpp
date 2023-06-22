#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	//delete spriteCommon_;
}

void Sprite::Inilialize(SpriteCommon* spriteCommon)
{
	//spriteCommon_ = new SpriteCommon;
	spriteCommon_ = spriteCommon;
}

void Sprite::Update()
{
}

void Sprite::Draw()
{
	spriteCommon_->Draw();
}
