#pragma once

/// <summary>
/// イージング用のnamespace
/// </summary>

namespace Easing 
{
	/// <summary>
	/// ラープ
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	double lerp(const double a, const double b, const double t);

	/// <summary>
	/// イーズインアウト
	/// </summary>
	/// <param name="startpos"></param>
	/// <param name="endpos"></param>
	/// <param name="time"></param>
	/// <param name="num"></param>
	/// <returns></returns>
	double EaseInOut(const double startpos, const double endpos, float time, float num);
	
	/// <summary>
	/// イーズインバック
	/// </summary>
	/// <param name="startpos"></param>
	/// <param name="endpos"></param>
	/// <param name="time"></param>
	/// <param name="num"></param>
	/// <param name="p1"></param>
	/// <returns></returns>
	double EaseInBack(const double startpos, const double endpos, float time, float num = 1, double p1 = 1.70158);
	
	/// <summary>
	/// イーズアウトバック
	/// </summary>
	/// <param name="startpos"></param>
	/// <param name="endpos"></param>
	/// <param name="time"></param>
	/// <param name="num"></param>
	/// <returns></returns>
	double EaseOutBack(const double startpos, const double endpos, float time, float num = 1);
	
	/// <summary>
	/// イーズイン
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="time"></param>
	/// <param name="num"></param>
	/// <returns></returns>
	double EaseIn(const double start, const double end, const double time, float num);
	
	/// <summary>
	/// イーズアウト
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="time"></param>
	/// <param name="num"></param>
	/// <returns></returns>
	double EaseOut(const double start, const double end, const double time, float num);
}

