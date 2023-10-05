#pragma once

class HitStop
{
private:
	float time = 0;
	bool countFlag;
public:
	HitStop();
	~HitStop();
	void Update();
	void SetTime(float time);
	float GetTime() { return time; };
};