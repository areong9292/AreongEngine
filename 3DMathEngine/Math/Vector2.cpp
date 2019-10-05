#include "Vector2.h"

namespace AreongEngine
{
	Vector2::Vector2()
	{
		x = 0.0;
		y = 0.0;
	}

	Vector2::Vector2(float vx, float vy)
	{
		x = vx;
		y = vy;
	}

	Vector2::~Vector2()
	{
	}

	void AreongEngine::Vector2::set(float vx, float vy)
	{
		this->x = vx;
		this->y = vy;
	}

	Vector2::Vector2(const Vector2& vec)
	{
		x = vec.x;
		y = vec.y;
	}

	Vector2& Vector2::operator=(const Vector2& vec)
	{
		x = vec.x;
		y = vec.y;

		return *this;
	}

	Vector2 Vector2::operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2& Vector2::operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;

		return *this;
	}

	Vector2 Vector2::operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	Vector2& Vector2::operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;

		return *this;
	}

	Vector2 Vector2::operator*(const float s) const
	{
		return Vector2(x * s, y * s);
	}

	void Vector2::operator*=(const float s)
	{
		x = x * s;
		y = y * s;
	}

	Vector2 Vector2::operator/(const float s) const
	{
		return Vector2(x / s, y / s);
	}

	void Vector2::operator/=(const float s)
	{
		x = x / s;
		y = y / s;
	}

	float Vector2::operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	bool Vector2::operator<(const Vector2 & vec) const
	{
		if (x == vec.x)
		{
			return y < vec.y;
		}

		return x < vec.x;
	}

	bool Vector2::operator==(const Vector2 & vec) const
	{
		return (x == vec.x) && (y == vec.y);
	}

	bool Vector2::operator!=(const Vector2 & vec) const
	{
		return !(*this == vec);
	}

	float Vector2::magnitude()
	{
		float mag = std::sqrt(x * x + y * y);
		return mag;
	}

	void Vector2::normalize()
	{
		float mag = std::sqrt(x * x + y * y);

		// can calc only vector's magnitude is bigger then 0
		if (mag > 0.0f)
		{
			float magInverse = 1.0f / mag;

			x = x * magInverse;
			y = y * magInverse;
		}
	}
}