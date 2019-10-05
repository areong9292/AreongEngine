#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <cmath>

namespace AreongEngine
{
	class Vector2
	{
	public:

		// 2-dimension vector component
		float x;
		float y;

		/// constructor
		Vector2();
		Vector2(float vx, float vy);

		// copy constructor
		Vector2(const Vector2& other);

		/// destructor
		~Vector2();

		void set(float vx, float vy);

		/// Operator Overloading
		Vector2& operator=(const Vector2& vec);
		
		Vector2 operator+(const Vector2& vec) const;
		Vector2& operator+=(const Vector2& vec);

		Vector2 operator-(const Vector2& vec) const;
		Vector2& operator-=(const Vector2& vec);

		Vector2 operator*(const float s) const;
		void operator*=(const float s);
		
		Vector2 operator/(const float s) const;
		void operator/=(const float s);
		
		// Vector dot
		float operator*(const Vector2& vec) const;

		bool operator<(const Vector2& vec) const;
		bool operator==(const Vector2& vec) const;
		bool operator!=(const Vector2& vec) const;

		// Vector magnitude
		float magnitude();

		// Vector normalize
		void normalize();
	};
}

#endif // !VECTOR2_H_
