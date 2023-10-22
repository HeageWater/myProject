#include "HitStop.h"

HitStop::HitStop()
{
	time_ = 0;
	countFlag_ = false;
}

HitStop::~HitStop()
{
}

void HitStop::Update()
{
	if (countFlag_)
	{
		time_--;
	}

	if (time_ < 0)
	{
		countFlag_ = false;
	}
}

void HitStop::SetTime(float time)
{
	this->time_ = time;
	countFlag_ = true;
};
