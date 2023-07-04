#pragma once
#include "Sprite.h"

class Post :Sprite
{
	Post();

	void Draw(ID3D12GraphicsCommandList* cmdList);
};