#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cmath>

namespace AreongEngine
{
	class Vector3
	{
	public:

		// 3-dimension vector component
		float x;
		float y;
		float z;


		static const Vector3& zero;
		static const Vector3& one;

		static const Vector3& up;
		static const Vector3& down;
		static const Vector3& left;
		static const Vector3& right;
		static const Vector3& forward;
		static const Vector3& back;

		// Linearly interpolates between two vectors(vec0, vec1)
		static Vector3 lerp(const Vector3& vec0, const Vector3& vec1, float delta);

		/// constructor
		Vector3();
		Vector3(float vx, float vy, float vz);

		// copy constructor
		Vector3(const Vector3& other);

		/// destructor
		~Vector3();

		void set(float vx, float vy, float vz);

		/// Operator Overloading
		Vector3& operator=(const Vector3& vec);
		
		Vector3 operator+(const Vector3& vec) const;
		Vector3& operator+=(const Vector3& vec);

		Vector3 operator-(const Vector3& vec) const;
		Vector3& operator-=(const Vector3& vec);

		Vector3 operator*(const float s) const;
		void operator*=(const float s);

		Vector3 operator/(const float s) const;
		void operator/=(const float s);

		// Vector dot
		float operator*(const Vector3& vec) const;
		float dot(const Vector3& vec) const;

		// Vector cross
		Vector3 cross(const Vector3& vec) const;
		void operator%=(const Vector3& vec);
		Vector3 operator%(const Vector3& vec) const;

		bool operator<(const Vector3& vec) const;
		bool operator==(const Vector3& vec) const;
		bool operator!=(const Vector3& vec) const;
		
		// Vector magnitude
		float magnitude();

		// Vector normalize
		void normalize();
	};
}

#endif // !VECTOR3_H_