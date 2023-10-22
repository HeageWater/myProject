#pragma once

/// <summary>
/// 
/// </summary>
class Matrix
{
public:
	float m_[4][4];
public:
	Matrix();

	/// <summary>
	/// 初期化
	/// </summary>
	void Identity();
	Matrix& operator*=(const Matrix& m2);
};

/// <summary>
/// 逆行列
/// </summary>
/// <param name="mat"></param>
/// <param name="invMat"></param>
/// <returns></returns>
bool InverseMatrix(const Matrix& mat, Matrix& invMat);

