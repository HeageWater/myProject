#pragma once
#include "MyMath.h"

class Shake
{
private:
	float time_ = 0;
	bool countFlag;
	Vector3D shake;
public:
	Shake();
	~Shake();
	void Update();
	void SetTime(float time);
	float GetTime() { return time_; };
};