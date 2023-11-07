 #pragma once
#include "MyMath.h"
#include "Input.h"

/// <summary>
/// デバッグ用カメラ,まだそこまで機能はない
/// </summary>
class MyDebugCamera
{
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
	Vector3D Transfrom() { return eye_; }

	/// <summary>
	/// カメラの前方向を取得
	/// </summary>
	/// <returns></returns>
	Vector3D FrontVec() { return frontVec_; }

	/// <summary>
	/// eyeとtargetを動かす
	/// </summary>
	/// <param name="camerapos"></param>
	void Move(float camerapos);

public:
	Matrix mat_;
	Vector3D eye_;		//	視点座標
	Vector3D target_;	//	注視点座標
	Vector3D up_;		//	上方向ベクトル
	Matrix billboard_;
private:

	//ターゲット
	float disEyeTarget_;

	//前ベクトル
	Vector3D frontVec_;

	//カーソル関係
	Vector2D cursor_;
	Vector2D prevCursor_;
	Vector2D moveCursor_;
	Vector2D cursorSpd_;

	//角度関係
	Vector2D rotAngle_;
	Vector2D prevRotAngle_;
	Vector2D endRotAngle_;

	//右ベクトル
	Vector3D rightVec_;
	//下ベクトル
	Vector3D downVec_;

	//キーボード
	Input* input_;
};

