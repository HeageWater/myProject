#include "MyDebugCamera.h"

MyDebugCamera::MyDebugCamera()
{

}

MyDebugCamera::MyDebugCamera(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	Init(_eye, _target, _up);
	frontVec_ = target_ - eye_;
	disEyeTarget_ = frontVec_.length();
}

void MyDebugCamera::Update()
{
	input_ = Input::GetInstance();

	eye_.x_ += input_->GetKey(DIK_RIGHT) - input_->GetKey(DIK_LEFT);
	eye_.y_ += input_->GetKey(DIK_UP) - input_->GetKey(DIK_DOWN);
	eye_.z_ += input_->GetKey(DIK_O) - input_->GetKey(DIK_P);/*

	if (input.Click(Input::LeftClick))
	{
		eye.x += (input.CursorPos().x - (float)640) / 100;
		eye.y += (input.CursorPos().y - (float)320) / 100;
	}*/

	MatUpdate();
}

void MyDebugCamera::Init(Vector3D _eye, Vector3D _target, Vector3D _up)
{
	eye_ = _eye;
	target_ = _target;
	up_ = _up;

	MatUpdate();
}

void MyDebugCamera::MatUpdate()
{
	mat_ = MyMath::LookAtLH(eye_, target_, up_);
}

void MyDebugCamera::Move(float camerapos)
{
	eye_.x_ = camerapos;
	target_.x_ = camerapos;

	MatUpdate();
}
