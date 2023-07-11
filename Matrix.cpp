#include "Matrix.h"
#include <cmath>

#define MAX_ERR 1e-10

Matrix::Matrix()
{
	Normalize();
}

void Matrix::Normalize()
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			float temp = 0;

			if (i == j) 
			{
				temp = 1;
			}

			m[j][i] = temp;
		}
	}
}

Matrix& Matrix::operator*=(const Matrix& m2)
{
	Matrix prev(*this);

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m[i][j] = prev.m[i][0] * m2.m[0][j] + prev.m[i][1] * m2.m[1][j] + prev.m[i][2] * m2.m[2][j] + prev.m[i][3] * m2.m[3][j];
		}
	}

	return *this;
}

bool InverseMatrix(const Matrix& mat, Matrix& invMat)
{
    float sweep[4][8];
    int k;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            // sweepの左側に逆行列を求める行列をセット
            sweep[y][x] = mat.m[y][x];

            // sweepの右側に単位行列をセット
            if (y == x) {
                sweep[y][4 + x] = 1;
            }
            else {
                sweep[y][4 + x] = 0;
            }
        }
    }


    /* 全ての列の対角成分に対する繰り返し */
    for (k = 0; k < 4; k++) {

        /* 最大の絶対値を注目対角成分の絶対値と仮定 */
        double max = fabs(sweep[k][k]);
        int max_i = k;

        /* k列目が最大の絶対値となる行を探す */
        for (int y = k + 1; y < 4; y++) {
            if (fabs(sweep[y][k]) > max) {
                max = fabs(sweep[y][k]);
                max_i = y;
            }
        }

        if (fabs(sweep[max_i][k]) <= MAX_ERR) {
            /* 逆行列は求められない */
            return false;
        }

        /* 操作（１）：k行目とmax_i行目を入れ替える */
        if (k != max_i) {
            for (int x = 0; x < 8; x++) {
                double tmp = sweep[max_i][x];
                sweep[max_i][x] = sweep[k][x];
                sweep[k][x] = tmp;
            }
        }

        /* sweep[k][k]に掛けると1になる値を求める */
        float a = 1 / sweep[k][k];

        /* 操作（２）：k行目をa倍する */
        for (int x = 0; x < 8; x++) {
            /* これによりsweep[k][k]が1になる */
            sweep[k][x] *= a;
        }

        /* 操作（３）によりk行目以外の行のk列目を0にする */
        for (int y = 0; y < 4; y++) {
            if (y == k) {
                /* k行目はそのまま */
                continue;
            }

            /* k行目に掛ける値を求める */
            a = -sweep[y][k];

            for (int x = 0; x < 8; x++) {
                /* i行目にk行目をa倍した行を足す */
                /* これによりsweep[i][k]が0になる */
                sweep[y][x] += sweep[k][x] * a;
            }
        }
    }

    /* sweepの右半分がmatの逆行列 */
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            invMat.m[y][x] = sweep[y][4 + x];
        }
    }

    return true;
}
