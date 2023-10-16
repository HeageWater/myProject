#pragma once
#include "MyMath.h"
#include "Input.h"

/// <summary>
/// デバッグ用カメラ,まだそこまで機能はない
/// </summary>
class MyDebugCamera
{
public:
	Matrix mat;
	Vector3D eye;		//	視点座標
	Vector3D target;	//	注視点座標
	Vector3D up;		//	上方向ベクトル
	Matrix billboard;
private:
	float disEyeTarget;
	Vector3D frontVec;
	Vector2D cursor;
	Vector2D prevCursor;
	Vector2D moveCursor;
	Vector2D cursorSpd;

	Vector2D rotAngle;
	Vector2D prevRotAngle;
	Vector2D endRotAngle;

	Vector3D rightVec;
	Vector3D downVec;

	Input* input_;
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MyDebugCamera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	MyDebugCamera(Vector3D _eye, Vector3D _target, Vector3D _up);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_eye"></param>
	/// <param name="_target"></param>
	/// <param name="_up"></param>
	void Init(Vector3D _eye, Vector3D _target, Vector3D _up);

	/// <summary>
	/// 更新
	/// </summary>
	void MatUpdate();

	/// <summary>
	/// eyeを取得
	/// </summary>
	/// <returns></returns>
	Vector3D Transfrom() { return eye; }

	/// <summary>
	/// カメラの前方向を取得
	/// </summary>
	/// <returns></returns>
	Vector3D FrontVec() { return frontVec; }

	/// <summary>
	/// eyeとtargetを動かす
	/// </summary>
	/// <param name="camerapos"></param>
	void Move(float camerapos);
};

