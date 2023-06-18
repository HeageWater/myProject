#include "MyDebugCamera.h"

MyDebugCamera::MyDebugCamera(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	Init(_eye, _target, _up);
	frontVec = target - eye;
	disEyeTarget = frontVec.length();
}

void MyDebugCamera::Update(Input& input)
{
	eye.x_ += (float)(input.GetKey(DIK_RIGHT) - input.GetKey(DIK_LEFT));
	eye.y_ += (float)(input.GetKey(DIK_UP) - input.GetKey(DIK_DOWN));
	eye.z_ += (float)(input.GetKey(DIK_O) - input.GetKey(DIK_P));/*

	if (input.Click(Input::LeftClick))
	{
		eye.x += (input.CursorPos().x - (float)640) / 100;
		eye.y += (input.CursorPos().y - (float)320) / 100;
	}*/

	MatUpdate();
}

void MyDebugCamera::Init(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	eye = _eye;
	target = _target;
	up = _up;

	MatUpdate();
}

void MyDebugCamera::MatUpdate()
{
	mat = MyMath::LookAtLH(eye, target, up);
}

void MyDebugCamera::Move(float camerapos)
{
	eye.x_ = camerapos;
	target.x_ = camerapos;

	MatUpdate();
}
