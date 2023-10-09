#pragma once
namespace Easing {
	double lerp(const double a, const double b, const double t);

	double EaseInOut(const double startpos, const double endpos, float time, float num);
	double EaseInBack(const double startpos, const double endpos, float time, float num = 1, double p1 = 1.70158);
	double EaseOutBack(const double startpos, const double endpos, float time, float num = 1);
	double EaseIn(const double start, const double end, const double time, float num);
	double EaseOut(const double start, const double end, const double time, float num);
}

