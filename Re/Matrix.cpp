#include "Matrix.h"
#include <cmath>

#define MAX_ERR 1e-10

Matrix::Matrix()
{
	Identity();
}

void Matrix::Identity()
{
	for (size_t x = 0; x < 4; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			if (x == y) m[y][x] = 1.0f;
			else		m[y][x] = 0.0f;
		}
	}
}

Matrix& Matrix::operator*=(const Matrix& m2)
{
	Matrix prev(*this);

	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 4; x++)
		{
			m[y][x] = prev.m[y][0] * m2.m[0][x] + prev.m[y][1] * m2.m[1][x] + prev.m[y][2] * m2.m[2][x] + prev.m[y][3] * m2.m[3][x];
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
            // sweep�̍����ɋt�s������߂�s����Z�b�g
            sweep[y][x] = mat.m[y][x];

            // sweep�̉E���ɒP�ʍs����Z�b�g
            if (y == x) {
                sweep[y][4 + x] = 1;
            }
            else {
                sweep[y][4 + x] = 0;
            }
        }
    }


    /* �S�Ă̗�̑Ίp�����ɑ΂���J��Ԃ� */
    for (k = 0; k < 4; k++) {

        /* �ő�̐�Βl�𒍖ڑΊp�����̐�Βl�Ɖ��� */
        double max = fabs(sweep[k][k]);
        int max_i = k;

        /* k��ڂ��ő�̐�Βl�ƂȂ�s��T�� */
        for (int y = k + 1; y < 4; y++) {
            if (fabs(sweep[y][k]) > max) {
                max = fabs(sweep[y][k]);
                max_i = y;
            }
        }

        if (fabs(sweep[max_i][k]) <= MAX_ERR) {
            /* �t�s��͋��߂��Ȃ� */
            return false;
        }

        /* ����i�P�j�Fk�s�ڂ�max_i�s�ڂ����ւ��� */
        if (k != max_i) {
            for (int x = 0; x < 8; x++) {
                double tmp = sweep[max_i][x];
                sweep[max_i][x] = sweep[k][x];
                sweep[k][x] = tmp;
            }
        }

        /* sweep[k][k]�Ɋ|�����1�ɂȂ�l�����߂� */
        float a = 1 / sweep[k][k];

        /* ����i�Q�j�Fk�s�ڂ�a�{���� */
        for (int x = 0; x < 8; x++) {
            /* ����ɂ��sweep[k][k]��1�ɂȂ� */
            sweep[k][x] *= a;
        }

        /* ����i�R�j�ɂ��k�s�ڈȊO�̍s��k��ڂ�0�ɂ��� */
        for (int y = 0; y < 4; y++) {
            if (y == k) {
                /* k�s�ڂ͂��̂܂� */
                continue;
            }

            /* k�s�ڂɊ|����l�����߂� */
            a = -sweep[y][k];

            for (int x = 0; x < 8; x++) {
                /* i�s�ڂ�k�s�ڂ�a�{�����s�𑫂� */
                /* ����ɂ��sweep[i][k]��0�ɂȂ� */
                sweep[y][x] += sweep[k][x] * a;
            }
        }
    }

    /* sweep�̉E������mat�̋t�s�� */
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            invMat.m[y][x] = sweep[y][4 + x];
        }
    }

    return true;
}
