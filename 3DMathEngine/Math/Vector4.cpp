#include "Vector3.h"
#include "Vector4.h"

namespace AreongEngine
{
	Vector4::Vector4()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		w = 0.0;
	}

	Vector4::Vector4(float vx, float vy, float vz, float vw)
	{
		x = vx;
		y = vy;
		z = vz;
		w = vw;
	}

	Vector4::~Vector4()
	{
	}

	void Vector4::set(float vx, float vy, float vz, float vw)
	{
		this->x = vx;
		this->y = vy;
		this->z = vz;
		this->w = vw;
	}

	Vector4::Vector4(const Vector4& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	Vector4& Vector4::operator=(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = 1.0f;

		return *this;
	}

	Vector4& Vector4::operator=(const Vector4& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;

		return *this;
	}

	Vector4 Vector4::operator+(const Vector4& vec) const
	{
		return Vector4(x + vec.x,
			y + vec.y,
			z + vec.z,
			w + vec.w);
	}

	Vector4& Vector4::operator+=(const Vector4& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;

		return *this;
	}

	Vector4 Vector4::operator-(const Vector4& vec) const
	{
		return Vector4(x - vec.x,
			y - vec.y,
			z - vec.z,
			w - vec.w);
	}

	Vector4 & Vector4::operator-=(const Vector4 & vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;

		return *this;
	}

	Vector4 Vector4::operator*(const float s) const
	{
		return Vector4(x * s, y * s, z * s, w * s);
	}

	void Vector4::operator*=(const float s)
	{
		x = x * s;
		y = y * s;
		z = z * s;
		w = w * s;
	}

	Vector4 Vector4::operator/(const float s) const
	{
		return Vector4(x / s, y / s, z / s, w / s);
	}

	void Vector4::operator/=(const float s)
	{
		x = x / s;
		y = y / s;
		z = z / s;
		w = w / s;
	}

	bool Vector4::operator<(const Vector4 & vec) const
	{
		if (x == vec.x)
		{
			if (y == vec.y)
			{
				if (z == vec.z)
				{
					return w < vec.w;
				}

				return z < vec.z;
			}

			return y < vec.y;
		}

		return x < vec.x;
	}

	bool Vector4::operator==(const Vector4 & vec) const
	{
		return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
	}

	bool Vector4::operator!=(const Vector4 & vec) const
	{
		return !(*this == vec);
	}

	float Vector4::magnitude()
	{
		float mag = std::sqrt(x * x + y * y + z * z);
		return mag;
	}

	void Vector4::normalize()
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