#pragma once
#include "MyMath.h"

class Shake
{
private:
	float time = 0;
	bool countFlag;
	Vector3D shake;
public:
	Shake();
	~Shake();
	void Update();
	void SetTime(float time);
	float GetTime() { return time; };
};