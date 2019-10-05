#include "Matrix.h"

namespace AreongEngine
{
	// zero, identity ���� ������ ���� ȣ��
	const Matrix Matrix::zero(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);

	const Matrix Matrix::identity(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	const int Matrix::MATRIX_SIZE = sizeof(_m);

	Matrix::Matrix()
	{
		*this = identity;
		//cout << "����Ʈ ������ ��ü���� : " << this << endl;
	}

	Matrix::Matrix(float m00, float m01, float m02, float m03,
				   float m10, float m11, float m12, float m13,
				   float m20, float m21, float m22, float m23,
				   float m30, float m31, float m32, float m33)
	{
		_m[0]  = m00; _m[1]  = m01; _m[2]  = m02; _m[3]  = m03;
		_m[4]  = m10; _m[5]  = m11; _m[6]  = m12; _m[7]  = m13;
		_m[8]  = m20; _m[9]  = m21; _m[10] = m22; _m[11] = m23;
		_m[12] = m30; _m[13] = m31; _m[14] = m32; _m[15] = m33;

		//cout << "��ü���� : " << this << endl;
	}

	Matrix::~Matrix()
	{
		//cout << "��ü�Ҹ� : " << this << endl;
	}

	float& Matrix::operator[](int i)
	{
		return _m[i];
	}

	float Matrix::operator[](int i) const
	{
		return _m[i];
	}

	Matrix::Matrix(const Matrix& mat)
	{
		memcpy(_m, mat._m, MATRIX_SIZE);
		//cout << "��������� ȣ�� : " << this << "----" << &mat << endl;
	}

	Matrix& Matrix::operator=(const Matrix& mat)
	{
		memcpy(_m, mat._m, MATRIX_SIZE);
		//cout << "= ȣ�� : " << this << "----" <<&mat << endl;
		return *this;
	}

	Matrix Matrix::operator+(const Matrix& mat) const
	{
		//cout << "���ϱ� : " << this << "----" << &mat << endl;

		// �̷� ������ ��ü�� �����ϰ� ������ ��� ����Ʈ ������ ȣ��(Matrix test)�ǰ�
		// ��������ڰ� ȣ��ȴ�(test��� ��ü�� ������� �ش� ���� �ӽ� ��ü�� �����)
		// �׳� ���� ����� �� ��ü �ϳ��� ����� ���ʿ��� ȣ���� �ٱ� ������ �̷��� ó��
		/*
		Matrix test;
		test._m[0] = _m[0] + mat._m[0];

		return test;
		*/
		
		return Matrix(
			_m[0] + mat._m[0], _m[1] + mat._m[1], _m[2] + mat._m[2], _m[3] + mat._m[3], 
			_m[4] + mat._m[4], _m[5] + mat._m[5], _m[6] + mat._m[6], _m[7] + mat._m[7], 
			_m[8] + mat._m[8], _m[9] + mat._m[9], _m[10] + mat._m[10], _m[11] + mat._m[11], 
			_m[12] + mat._m[12], _m[13] + mat._m[13], _m[14] + mat._m[14], _m[15] + mat._m[15]);
	}

	Matrix& Matrix::operator+=(const Matrix& mat)
	{
		_m[0] = _m[0] + mat._m[0];
		_m[1] = _m[1] + mat._m[1];
		_m[2] = _m[2] + mat._m[2];
		_m[3] = _m[3] + mat._m[3];

		_m[4] = _m[4] + mat._m[4];
		_m[5] = _m[5] + mat._m[5];
		_m[6] = _m[6] + mat._m[6];
		_m[7] = _m[7] + mat._m[7];

		_m[8] = _m[8] + mat._m[8];
		_m[9] = _m[9] + mat._m[9];
		_m[10] = _m[10] + mat._m[10];
		_m[11] = _m[11] + mat._m[11];

		_m[12] = _m[12] + mat._m[12];
		_m[13] = _m[13] + mat._m[13];
		_m[14] = _m[14] + mat._m[14];
		_m[15] = _m[15] + mat._m[15];

		return *this;
	}

	Matrix Matrix::operator-(const Matrix& mat) const
	{
		return Matrix(
			_m[0] - mat._m[0], _m[1] - mat._m[1], _m[2] - mat._m[2], _m[3] - mat._m[3],
			_m[4] - mat._m[4], _m[5] - mat._m[5], _m[6] - mat._m[6], _m[7] - mat._m[7],
			_m[8] - mat._m[8], _m[9] - mat._m[9], _m[10] - mat._m[10], _m[11] - mat._m[11],
			_m[12] - mat._m[12], _m[13] - mat._m[13], _m[14] - mat._m[14], _m[15] - mat._m[15]);
	}

	Matrix& Matrix::operator-=(const Matrix& mat)
	{
		_m[0] = _m[0] - mat._m[0];
		_m[1] = _m[1] - mat._m[1];
		_m[2] = _m[2] - mat._m[2];
		_m[3] = _m[3] - mat._m[3];

		_m[4] = _m[4] - mat._m[4];
		_m[5] = _m[5] - mat._m[5];
		_m[6] = _m[6] - mat._m[6];
		_m[7] = _m[7] - mat._m[7];

		_m[8] = _m[8] - mat._m[8];
		_m[9] = _m[9] - mat._m[9];
		_m[10] = _m[10] - mat._m[10];
		_m[11] = _m[11] - mat._m[11];

		_m[12] = _m[12] - mat._m[12];
		_m[13] = _m[13] - mat._m[13];
		_m[14] = _m[14] - mat._m[14];
		_m[15] = _m[15] - mat._m[15];

		return *this;
	}

	Matrix Matrix::operator*(const float s) const
	{
		return Matrix(
			_m[0] * s, _m[1] * s, _m[2] * s, _m[3] * s,
			_m[4] * s, _m[5] * s, _m[6] * s, _m[7] * s,
			_m[8] * s, _m[9] * s, _m[10] * s, _m[11] * s,
			_m[12] * s, _m[13] * s, _m[14] * s, _m[15] * s);
	}

	void Matrix::operator*=(const float s)
	{
		_m[0] = _m[0] * s;
		_m[1] = _m[1] * s;
		_m[2] = _m[2] * s;
		_m[3] = _m[3] * s;

		_m[4] = _m[4] * s;
		_m[5] = _m[5] * s;
		_m[6] = _m[6] * s;
		_m[7] = _m[7] * s;

		_m[8] = _m[8] * s;
		_m[9] = _m[9] * s;
		_m[10] = _m[10] * s;
		_m[11] = _m[11] * s;

		_m[12] = _m[12] * s;
		_m[13] = _m[13] * s;
		_m[14] = _m[14] * s;
		_m[15] = _m[15] * s;
	}

	Matrix Matrix::operator*(const Matrix & mat) const
	{
		const float* m0 = _m;
		const float* m1 = mat._m;

		/*Matrix result;

		result._m[0] = m0[0] * m1[0] + m0[1] * m1[4] + m0[2] * m1[8] + m0[3] * m1[12];
		result._m[1] = m0[0] * m1[1] + m0[1] * m1[5] + m0[2] * m1[9] + m0[3] * m1[13];
		result._m[2] = m0[0] * m1[2] + m0[1] * m1[6] + m0[2] * m1[10] + m0[3] * m1[14];
		result._m[3] = m0[0] * m1[3] + m0[1] * m1[7] + m0[2] * m1[11] + m0[3] * m1[15];

		result._m[4] = m0[4] * m1[0] + m0[5] * m1[4] + m0[6] * m1[8] + m0[7] * m1[12];
		result._m[5] = m0[4] * m1[1] + m0[5] * m1[5] + m0[6] * m1[9] + m0[7] * m1[13];
		result._m[6] = m0[4] * m1[2] + m0[5] * m1[6] + m0[6] * m1[10] + m0[7] * m1[14];
		result._m[7] = m0[4] * m1[3] + m0[5] * m1[7] + m0[6] * m1[11] + m0[7] * m1[15];

		result._m[8] = m0[8] * m1[0] + m0[9] * m1[4] + m0[10] * m1[8] + m0[11] * m1[12];
		result._m[9] = m0[8] * m1[1] + m0[9] * m1[5] + m0[10] * m1[9] + m0[11] * m1[13];
		result._m[10] = m0[8] * m1[2] + m0[9] * m1[6] + m0[10] * m1[10] + m0[11] * m1[14];
		result._m[11] = m0[8] * m1[3] + m0[9] * m1[7] + m0[10] * m1[11] + m0[11] * m1[15];

		result._m[12] = m0[12] * m1[0] + m0[13] * m1[4] + m0[14] * m1[8] + m0[15] * m1[12];
		result._m[13] = m0[12] * m1[1] + m0[13] * m1[5] + m0[14] * m1[9] + m0[15] * m1[13];
		result._m[14] = m0[12] * m1[2] + m0[13] * m1[6] + m0[14] * m1[10] + m0[15] * m1[14];
		result._m[15] = m0[12] * m1[3] + m0[13] * m1[7] + m0[14] * m1[11] + m0[15] * m1[15];
		*/

		return Matrix(
			m0[0] * m1[0] + m0[1] * m1[4] + m0[2] * m1[8] + m0[3] * m1[12],
			m0[0] * m1[1] + m0[1] * m1[5] + m0[2] * m1[9] + m0[3] * m1[13],
			m0[0] * m1[2] + m0[1] * m1[6] + m0[2] * m1[10] + m0[3] * m1[14],
			m0[0] * m1[3] + m0[1] * m1[7] + m0[2] * m1[11] + m0[3] * m1[15],
			
			m0[4] * m1[0] + m0[5] * m1[4] + m0[6] * m1[8] + m0[7] * m1[12],
			m0[4] * m1[1] + m0[5] * m1[5] + m0[6] * m1[9] + m0[7] * m1[13],
			m0[4] * m1[2] + m0[5] * m1[6] + m0[6] * m1[10] + m0[7] * m1[14],
			m0[4] * m1[3] + m0[5] * m1[7] + m0[6] * m1[11] + m0[7] * m1[15],
			
			m0[8] * m1[0] + m0[9] * m1[4] + m0[10] * m1[8] + m0[11] * m1[12],
			m0[8] * m1[1] + m0[9] * m1[5] + m0[10] * m1[9] + m0[11] * m1[13],
			m0[8] * m1[2] + m0[9] * m1[6] + m0[10] * m1[10] + m0[11] * m1[14],
			m0[8] * m1[3] + m0[9] * m1[7] + m0[10] * m1[11] + m0[11] * m1[15],
			
			m0[12] * m1[0] + m0[13] * m1[4] + m0[14] * m1[8] + m0[15] * m1[12],
			m0[12] * m1[1] + m0[13] * m1[5] + m0[14] * m1[9] + m0[15] * m1[13],
			m0[12] * m1[2] + m0[13] * m1[6] + m0[14] * m1[10] + m0[15] * m1[14],
			m0[12] * m1[3] + m0[13] * m1[7] + m0[14] * m1[11] + m0[15] * m1[15]);

		//return result;
	}

	Matrix& Matrix::operator*=(const Matrix& mat)
	{
		*this = *this * mat;
		return *this;
	}

	// ����� ����� �� ����... �׳� �� ����� ������?
	Matrix Matrix::getInverseMatrix()
	{
		int i;

		Matrix result = zero;

		result = getCofactorMatrix(*this, 4);

		result = getTransposeMatrix(result);

		for (i = 0; i < _dimension * _dimension; i++)
		{
			result[i] /= calcDeterminant(*this, 4);
		}

		return result;
	}

	// ��Ľ� ���ϴ� �޼ҵ�
	float Matrix::calcDeterminant(const Matrix& mat, int dimension)
	{
		int i;
		float det = 0;

		// 1���� = ��� �Ѱ�
		if (dimension == 1)
		{
			return mat[0];
		}

		// 2���� = ��� �ΰ�
		else if (dimension == 2)
		{
			// 0 1 ...
			// 4 5 ...
			// . .
			return mat[0] * mat[5] - mat[1] * mat[4];
		}

		// 3���� �̻� - 4���� �ʰ��� �������� �ʴ´�
		else
		{
			for (i = 0; i < mat._dimension; i++)
			{
				det = det + mat[i] * getCofactor(mat, dimension, 0, i);
			}

			return det;
		}

		return 0;
	}

	// row�� col�� ���μ� ���ϴ� �޼ҵ�
	float Matrix::getCofactor(const Matrix& mat, int dimension, int row, int col)
	{
		return (row + col) % 2 == 0 ?
			calcDeterminant(MinorMat(mat, dimension, row, col), dimension - 1)
			: (-calcDeterminant(MinorMat(mat, dimension, row, col), dimension - 1));

	}

	// ���μ� ��� ���ϴ� �޼ҵ�
	Matrix Matrix::getCofactorMatrix(const Matrix& mat, int dimension)
	{
		Matrix result = zero;
		int i;

		int oriDimension = mat._dimension;

		for (i = 0;i < oriDimension * oriDimension; i++)
		{
			// ��� ��� ���� 0���� ó��
			if (i % oriDimension >= dimension ||
				i / oriDimension >= dimension)
				result[i] = 0;
			else
				result[i] = getCofactor(mat, dimension, i / oriDimension, i % oriDimension);
		}

		return result;
	}

	// row�� col���� ���� ����� ���ϴ� �޼ҵ�
	Matrix Matrix::MinorMat(const Matrix& majorMat, int dimension, int row, int col)
	{
		// 0��ķ� �ʱ�ȭ
		Matrix minorMat = zero;

		int index = 0, i = 0;

		int oriDimension = majorMat._dimension;

		for (i = 0;i < oriDimension * oriDimension; i++)
		{
			// ���ڷ� ���� ������ ��� ��� ����
			if (i % oriDimension >= dimension ||
				i / oriDimension >= dimension)
				continue;

			// ���� ��� ��� ��Ҹ� ���鼭 row�� col���� �ƴϸ�
			if (i % oriDimension != col && i / oriDimension != row)
			{
				// ����Ŀ� �ְ� index ������Ų��
				minorMat[index++] = majorMat[i];

				// ����� �� �ְ� ���� �κ��� �ǳʶڴ�
				if (index % oriDimension >= dimension - 1)
				{
					index += oriDimension - index % oriDimension;
				}
			}
		}

		return minorMat;
	}

	Matrix Matrix::getTransposeMatrix(const Matrix& mat)
	{
		Matrix result = zero;
		int i, j;
		int oriDimension = mat._dimension;

		for (i = 0; i < oriDimension; i++) {
			for (j = 0; j < oriDimension; j++)
				result[i * oriDimension + j] = mat[j * oriDimension + i];
		}

		return result;
	}

	void Matrix::show()
	{
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				cout << _m[j + i * 4] << ", ";
			}
			cout << endl;
		}
	}
}