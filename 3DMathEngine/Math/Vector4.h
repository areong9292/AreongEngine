#ifndef VECTOR4_H_
#define VECTOR4_H_

#include <cmath>

namespace AreongEngine
{
	class Vector3;
	class Vector4
	{
	public:

		// 4-dimension vector component
		float x;
		float y;
		float z;
		float w;

		/// constructor
		Vector4();
		Vector4(float vx, float vy, float vz, float vw);

		// copy constructor
		Vector4(const Vector4& other);

		/// destructor
		~Vector4();

		void set(float vx, float vy, float vz, float vw);

		/// Operator Overloading
		Vector4& operator=(const Vector3& vec);
		Vector4& operator=(const Vector4& vec);

		Vector4 operator+(const Vector4& vec) const;
		Vector4& operator+=(const Vector4& vec);

		Vector4 operator-(const Vector4& vec) const;
		Vector4& operator-=(const Vector4& vec);

		Vector4 operator*(const float s) const;
		void operator*=(const float s);

		Vector4 operator/(const float s) const;
		void operator/=(const float s);

		bool operator<(const Vector4& vec) const;
		bool operator==(const Vector4& vec) const;
		bool operator!=(const Vector4& vec) const;

		// Vector magnitude
		float magnitude();

		// Vector normalize
		void normalize();
	};
}

#endif // !VECTOR4_H_