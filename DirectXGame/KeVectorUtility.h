#pragma once

#include "./Math/Vector3.h"
#include "./Math/Vector4.h"
#include "KamataEngine.h"


inline constexpr KamataEngine::Vector3 operator*(const float& scalar, const KamataEngine::Vector3& vector) { return KamataEngine::Vector3{scalar * vector.x, scalar * vector.y, scalar * vector.z}; }

inline constexpr KamataEngine::Vector3 operator*(const KamataEngine::Vector3& vector, const float& scalar) { return KamataEngine::Vector3{scalar * vector.x, scalar * vector.y, scalar * vector.z}; }

inline constexpr void operator+=(KamataEngine::Vector3& meVector, const KamataEngine::Vector3& otherVector) {

	meVector.x += otherVector.x;
	meVector.y += otherVector.y;
	meVector.z += otherVector.z;
}

inline constexpr KamataEngine::Vector3 operator+(const KamataEngine::Vector3& meVector, const KamataEngine::Vector3& otherVector) {

	return KamataEngine::Vector3{meVector.x + otherVector.x, meVector.y + otherVector.y, meVector.z + otherVector.z};
}

inline constexpr void operator-=(KamataEngine::Vector3& meVector, const KamataEngine::Vector3& otherVector) {

	meVector.x -= otherVector.x;
	meVector.y -= otherVector.y;
	meVector.z -= otherVector.z;
}

inline constexpr KamataEngine::Vector3 operator-(const KamataEngine::Vector3& meVector, const KamataEngine::Vector3& otherVector) {

	return KamataEngine::Vector3{meVector.x - otherVector.x, meVector.y - otherVector.y, meVector.z - otherVector.z};
}

inline constexpr KamataEngine::Vector3 ToKamataEngine(const Atrum::Math::Vector3& vector) { return KamataEngine::Vector3{vector.x, vector.y, vector.z}; }

inline constexpr Atrum::Math::Vector3 FromKamataEngine(const KamataEngine::Vector3& vector) { return Atrum::Math::Vector3{vector.x, vector.y, vector.z}; }

inline constexpr KamataEngine::Vector4 ToKamataEngine(const Atrum::Math::Vector4& vector) { return KamataEngine::Vector4{vector.x, vector.y, vector.z, vector.w}; }

inline constexpr Atrum::Math::Vector4 FromKamataEngine(const KamataEngine::Vector4& vector) { return Atrum::Math::Vector4{vector.x, vector.y, vector.z, vector.w}; }