#pragma once
#include "MyMath.h"
#include "Input.h"

//カメラをプレイヤーの足場とその天井の幅に合わせる(上限は持たせる)
//カメラ端からちょっとだけ手前の場所を確認して足場がなかったらちょっとカメラを引く

/// <summary>
/// まだクラスだけ,ここにゲーム内のカメラの挙動を書く
/// </summary>

class GameCamera
{
public:
	//Matrix mat;
	//Vector3D eye;		//	視点座標
	//Vector3D target;	//	注視点座標
	//Vector3D up;		//	上方向ベクトル
	//Matrix billboard;
private:
	/*float disEyeTarget;
	Vector3D frontVec;
	Vector2D cursor;
	Vector2D prevCursor;
	Vector2D moveCursor;
	Vector2D cursorSpd;

	Vector2D rotAngle;
	Vector2D prevRotAngle;
	Vector2D endRotAngle;

	Vector3D rightVec;
	Vector3D downVec;*/

	//描画用行列
	MyMath::MatView matView_;

	Matrix matProjection_ = MyMath::PerspectiveFovLH(
		Window::window_width_, Window::window_height_,
		MyMath::ConvertToRad(70.0f), 0.1f, 1000.0f);
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_eye"></param>
	/// <param name="_target"></param>
	/// <param name="_up"></param>
	void Initialize(Vector3D _eye, Vector3D _target, Vector3D _up);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="camerapos"></param>
	void Move(float camerapos);

	/// <summary>
	/// MatViewをセットする
	/// </summary>
	void SetMatView(MyMath::MatView view) { matView_ = view; };

	/// <summary>
	/// Matrixをセットする 
	/// </summary>
	void SetMatrix(Matrix matrix) { matProjection_ = matrix; };

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	MyMath::MatView GetMatView() { return matView_; };

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Matrix GetMatrix() { return matProjection_; };

	//シングルトン
	static GameCamera* GetInstance()
	{
		static GameCamera gameCamera;
		return &gameCamera;
	}

public:
	//
	GameCamera() = default;
	~GameCamera() = default;

	//
	GameCamera& operator=(const GameCamera&) = delete;
	GameCamera(const GameCamera&) = delete;
};