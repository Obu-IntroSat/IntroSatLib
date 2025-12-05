#ifndef UTILS_VECTOR_H_
#define UTILS_VECTOR_H_


namespace IntroSatLib {

struct Vector3 {
	float X;
	float Y;
	float Z;
	
	Vector3 operator+(const Vector3& other) const {
		return {X + other.X, Y + other.Y, Z + other.Z};
	}
	
	Vector3 operator-(const Vector3& other) const {
		return {X - other.X, Y - other.Y, Z - other.Z};
	}
	
	Vector3 operator*(const float& scalar) const {
		return {X * scalar, Y * scalar, Z * scalar};
	}
	
	Vector3 operator/(const float& scalar) const {
		return {X / scalar, Y / scalar, Z / scalar};
	}
	
	Vector3& operator+=(const Vector3& other) {
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	
	Vector3& operator-=(const Vector3& other) {
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	
	Vector3& operator*=(const float& scalar) {
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}
	
	Vector3& operator/=(const float& scalar) {
		X /= scalar;
		Y /= scalar;
		Z /= scalar;
		return *this;
	}
	
	bool operator==(const Vector3& other) const {
		return X == other.X && Y == other.Y && Z == other.Z;
	}
	
	bool operator!=(const Vector3& other) const {
		return !(*this == other);
	}

	float norm() const {
		return sqrt(X * X + Y * Y + Z * Z);
	}
};

}

#endif UTILS_VECTOR_H_