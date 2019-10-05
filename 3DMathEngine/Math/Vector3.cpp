#include "Vector3.h"

namespace AreongEngine
{
	const Vector3& Vector3::zero = Vector3(0, 0, 0);
	const Vector3& Vector3::one = Vector3(1, 1, 1);

	const Vector3& Vector3::up = Vector3(0, 1.0f, 0);
	const Vector3& Vector3::down = Vector3(0, -1.0f, 0);
	const Vector3& Vector3::left = Vector3(-1.0f, 0, 0);
	const Vector3& Vector3::right = Vector3(1.0f, 0, 0);
	const Vector3& Vector3::forward = Vector3(0, 0, 1.0f);
	const Vector3& Vector3::back = Vector3(0, 0, -1.0f);

	Vector3 Vector3::lerp(const Vector3& vec0, const Vector3& vec1, float delta)
	{
		return vec0 * (1 - delta) + vec1 * delta;
	}

	Vector3::Vector3()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	Vector3::Vector3(float vx, float vy, float vz)
	{
		x = vx;
		y = vy;
		z = vz;
	}

	Vector3::~Vector3()
	{
	}

	void Vector3::set(float vx, float vy, float vz)
	{
		this->x = vx;
		this->y = vy;
		this->z = vz;
	}

	Vector3::Vector3(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	Vector3& Vector3::operator=(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	Vector3 Vector3::operator+(const Vector3& vec) const
	{
		return Vector3(x + vec.x,
			y + vec.y,
			z + vec.z);
	}

	Vector3& Vector3::operator+=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	Vector3 Vector3::operator-(const Vector3& vec) const
	{
		return Vector3(x - vec.x,
			y - vec.y,
			z - vec.z);
	}

	Vector3& Vector3::operator-=(const Vector3 & vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	Vector3 Vector3::operator*(const float s) const
	{
		return Vector3(x * s, y * s, z * s);
	}

	void Vector3::operator*=(const float s)
	{
		x = x * s;
		y = y * s;
		z = z * s;
	}

	Vector3 Vector3::operator/(const float s) const
	{
		return Vector3(x / s, y / s, z / s);
	}

	void Vector3::operator/=(const float s)
	{
		x = x / s;
		y = y / s;
		z = z / s;
	}

	float Vector3::operator*(const Vector3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	float Vector3::dot(const Vector3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	Vector3 Vector3::cross(const Vector3 & vec) const
	{
		return Vector3(y * vec.z - z * vec.y,
			z * vec.x - x * vec.z,
			x * vec.y - y * vec.x);
	}

	void Vector3::operator%=(const Vector3 & vec)
	{
		*this = cross(vec);
	}

	Vector3 Vector3::operator%(const Vector3 & vec) const
	{
		return Vector3(y * vec.z - z * vec.y,
			z * vec.x - x * vec.z,
			x * vec.y - y * vec.x);
	}

	bool Vector3::operator<(const Vector3 & vec) const
	{
		if (x == vec.x)
		{
			if (y == vec.y)
			{
				return z < vec.z;
			}

			return y < vec.y;
		}

		return x < vec.x;
	}

	bool Vector3::operator==(const Vector3 & vec) const
	{
		return (x == vec.x) && (y == vec.y) && (z == vec.z);
	}

	bool Vector3::operator!=(const Vector3 & vec) const
	{
		return !(*this == vec);
	}

	float Vector3::magnitude()
	{
		float mag = std::sqrt(x * x + y * y + z * z);
		return mag;
	}

	void Vector3::normalize()
	{
		float mag = std::sqrt(x * x + y * y + z * z);

		// can calc only vector's magnitude is bigger then 0
		if (mag > 0.0f)
		{
			float magInverse = 1.0f / mag;

			x = x * magInverse;
			y = y * magInverse;
			z = z * magInverse;
		}
	}
}