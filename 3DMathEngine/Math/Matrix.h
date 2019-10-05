#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <cstring>
#include <cmath>

#include "Vector3.h"
#include "Vector4.h"

using namespace std;

namespace AreongEngine
{
	class Matrix
	{
	public:
		static const Matrix zero;
		static const Matrix identity;


		Matrix();
		Matrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		);

		~Matrix();

		Matrix(const Matrix& mat);


		float& operator[](int i);
		float operator[](int i) const;

		// ��ü ���� �� ������ �����ϸ� ��������� ȣ�� �ȵ�
		Matrix& operator=(const Matrix& mat);

		Matrix operator+(const Matrix& mat) const;
		Matrix& operator+=(const Matrix& mat);

		Matrix operator-(const Matrix& mat) const;
		Matrix& operator-=(const Matrix& mat);

		Matrix operator*(const float s) const;
		void operator*=(const float s);

		Matrix operator*(const Matrix& mat) const;
		Matrix& operator*=(const Matrix& mat);

		// ����� �޼ҵ�
		Matrix setInverseMatrix(const Matrix& mat);
		Matrix getInverseMatrix();
		void invertMatrix();

		// ��Ľ� ��� �޼ҵ�
		float calcDeterminant(const Matrix& mat, int dimension);

		// row�� col���� ���� ���μ� ���ϴ� �޼ҵ�
		float getCofactor(const Matrix& mat, int dimension, int row, int col);

		// ���μ� ��� ���ϴ� �޼ҵ�
		Matrix getCofactorMatrix(const Matrix& mat, int dimension);

		// row�� col���� ���� ����� ���ϴ� �޼ҵ�
		Matrix MinorMat(const Matrix& majorMat, int dimension, int row, int col);

		// ��ġ��� ���ϴ� �޼ҵ�
		Matrix getTransposeMatrix(const Matrix& mat);

		void show();

	private:
		static const int MATRIX_SIZE;

		union
		{
			float _m[16];
			struct
			{
				float _m00; float _m01; float _m02; float _m03;
				float _m10; float _m11; float _m12; float _m13;
				float _m20; float _m21; float _m22; float _m23;
				float _m30; float _m31; float _m32; float _m33;
			};
		};

		// ����� ����
		int _dimension = 4;
	};
}

#endif // !MATRIX_H_
