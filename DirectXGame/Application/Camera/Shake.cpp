#include "Shake.h"

Shake::Shake()
{
	time_ = 0;
	countFlag = false;

	shake = { 0,0,0 };
}

Shake::~Shake()
{
}

void Shake::Update()
{

}

void Shake::SetTime(float time)
{
	time_ = time;
}
