#include "Shake.h"
#include "Mymath.h"

void Shake::Initalize()
{
	time_ = 0;
	countFlag_ = false;

	shake_ = { 0,0,0 };
}

void Shake::Update()
{
	if (time_ == 0)
	{
		shake_.x_ = 0;
		shake_.y_ = 0;
		shake_.z_ = 1;

		return;
	}

	if (time_ > 0)
	{
		time_--;
	}

	if ((size_t)time_ % 3 == 0)
	{
		size_t min = 1;
		size_t max = 10;

		shake_.z_ *= -1;

		shake_.x_ = (float)MyMath::GetRandom(min, max) * shake_.z_ / 10;
		shake_.y_ = (float)MyMath::GetRandom(min, max) * shake_.z_ / 10;
	}

}

void Shake::SetTime(float time)
{
	time_ = time;

	size_t min = 1;
	size_t max = 10;

	shake_.x_ = (float)MyMath::GetRandom(min, max) * shake_.z_ / 10;
	shake_.y_ = (float)MyMath::GetRandom(min, max) * shake_.z_ / 10;
	shake_.z_ = 1;
}

Shake* Shake::GetInstance()
{
	static Shake shake;
	return &shake;
}
