#pragma once
#include "MyMath.h"
#include "Input.h"

//カメラをプレイヤーの足場とその天井の幅に合わせる(上限は持たせる)
//カメラ端からちょっとだけ手前の場所を確認して足場がなかったらちょっとカメラを引く

class GameCamera
{
//public:
//	Matrix mat;
//	Vector3D eye;		//	視点座標
//	Vector3D target;	//	注視点座標
//	Vector3D up;		//	上方向ベクトル
//	Matrix billboard;
//private:
//	/*float disEyeTarget;
//	Vector3D frontVec;
//	Vector2D cursor;
//	Vector2D prevCursor;
//	Vector2D moveCursor;
//	Vector2D cursorSpd;
//
//	Vector2D rotAngle;
//	Vector2D prevRotAngle;
//	Vector2D endRotAngle;
//
//	Vector3D rightVec;
//	Vector3D downVec;*/
//public:
//	//GameCamera();
//	GameCamera(Vector3D _eye, Vector3D _target, Vector3D _up);
//	void Update();
//	//void Update(Input& input);
//	void Init(Vector3D _eye, Vector3D _target, Vector3D _up);
//	void MatUpdate();
//	Vector3D Transfrom() { return eye; }
//	Vector3D FrontVec() { return frontVec; }
//	void Move(float camerapos);
};