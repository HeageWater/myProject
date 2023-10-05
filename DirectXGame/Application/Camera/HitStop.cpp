#include "HitStop.h"

HitStop::HitStop()
{
	time = 0;
	countFlag = false;
}

HitStop::~HitStop()
{
}

void HitStop::Update()
{
	if (countFlag)
	{
		time--;
	}

	if (time < 0)
	{
		countFlag = false;
	}
}

void HitStop::SetTime(float time)
{
	this->time = time;
	countFlag = true;
};
