#include "Matrix3D.h"
#include <cassert>
#include <cmath>

Matrix4x4 Inverse(const Matrix4x4& matrix) {

	Matrix4x4 result = {};

	result.m[0][0] = Determinant3x3(matrix.m[1][1], matrix.m[1][2], matrix.m[1][3], matrix.m[2][1], matrix.m[2][2], matrix.m[2][3], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]);
	result.m[0][1] = -Determinant3x3(matrix.m[1][0], matrix.m[1][2], matrix.m[1][3], matrix.m[2][0], matrix.m[2][2], matrix.m[2][3], matrix.m[3][0], matrix.m[3][2], matrix.m[3][3]);
	result.m[0][2] = Determinant3x3(matrix.m[1][0], matrix.m[1][1], matrix.m[1][3], matrix.m[2][0], matrix.m[2][1], matrix.m[2][3], matrix.m[3][0], matrix.m[3][1], matrix.m[3][3]);
	result.m[0][3] = -Determinant3x3(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2], matrix.m[2][0], matrix.m[2][1], matrix.m[2][2], matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);

	float determinant = matrix.m[0][0] * result.m[0][0] + matrix.m[0][1] * result.m[0][1] + matrix.m[0][2] * result.m[0][2] + matrix.m[0][3] * result.m[0][3];

	if (determinant < 0.00001f) {

		return Matrix4x4{};

	}

	float invDet = 1.0f / determinant;

	result.m[0][0] *= invDet;
	result.m[0][1] *= invDet;
	result.m[0][2] *= invDet;
	result.m[0][3] *= invDet;

	result.m[1][0] = Determinant3x3(matrix.m[0][1], matrix.m[0][2], matrix.m[0][3], matrix.m[2][1], matrix.m[2][2], matrix.m[2][3], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]) * invDet;
	result.m[1][1] = Determinant3x3(matrix.m[0][0], matrix.m[0][2], matrix.m[0][3], matrix.m[2][0], matrix.m[2][2], matrix.m[2][3], matrix.m[3][0], matrix.m[3][2], matrix.m[3][3]) * invDet;
	result.m[1][2] = Determinant3x3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][3], matrix.m[2][0], matrix.m[2][1], matrix.m[2][3], matrix.m[3][0], matrix.m[3][1], matrix.m[3][3]) * invDet;
	result.m[1][3] = Determinant3x3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2], matrix.m[2][0], matrix.m[2][1], matrix.m[2][2], matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]) * invDet;

	result.m[2][0] = Determinant3x3(matrix.m[0][1], matrix.m[0][2], matrix.m[0][3], matrix.m[1][1], matrix.m[1][2], matrix.m[1][3], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]) * invDet;
	result.m[2][1] = Determinant3x3(matrix.m[0][0], matrix.m[0][2], matrix.m[0][3], matrix.m[1][0], matrix.m[1][2], matrix.m[1][3], matrix.m[3][0], matrix.m[3][2], matrix.m[3][3]) * invDet;
	result.m[2][2] = Determinant3x3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][3], matrix.m[1][0], matrix.m[1][1], matrix.m[1][3], matrix.m[3][0], matrix.m[3][1], matrix.m[3][3]) * invDet;
	result.m[2][3] = Determinant3x3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2], matrix.m[1][0], matrix.m[1][1], matrix.m[1][2], matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]) * invDet;

	result.m[3][0] = Determinant3x3(matrix.m[0][1], matrix.m[0][2], matrix.m[0][3], matrix.m[1][1], matrix.m[1][2], matrix.m[1][3], matrix.m[2][1], matrix.m[2][2], matrix.m[2][3]) * invDet;
	result.m[3][1] = Determinant3x3(matrix.m[0][0], matrix.m[0][2], matrix.m[0][3], matrix.m[1][0], matrix.m[1][2], matrix.m[1][3], matrix.m[2][0], matrix.m[2][2], matrix.m[2][3]) * invDet;
	result.m[3][2] = Determinant3x3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][3], matrix.m[1][0], matrix.m[1][1], matrix.m[1][3], matrix.m[2][0], matrix.m[2][1], matrix.m[2][3]) * invDet;
	result.m[3][3] = Determinant3x3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2], matrix.m[1][0], matrix.m[1][1], matrix.m[1][2], matrix.m[2][0], matrix.m[2][1], matrix.m[2][2]) * invDet;

	return result;

}

Matrix4x4 MakeXRotateMatrix(const float& angle) {

	Matrix4x4 result = {0.0f};

	result.m[0][0] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[1][1] = cosf(angle);
	result.m[1][2] = sinf(angle);
	result.m[2][1] = -sinf(angle);
	result.m[2][2] = cosf(angle);

	return result;

}

Matrix4x4 MakeYRotateMatrix(const float& angle) {

	Matrix4x4 result = {0.0f};

	result.m[1][1] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[0][0] = cosf(angle);
	result.m[0][2] = sinf(angle);
	result.m[2][0] = -sinf(angle);
	result.m[2][2] = cosf(angle);

	return result;

}

Matrix4x4 MakeZRotateMatrix(const float& angle) {

	Matrix4x4 result = {0.0f};

	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[0][0] = cosf(angle);
	result.m[0][1] = sinf(angle);
	result.m[1][0] = -sinf(angle);
	result.m[1][1] = cosf(angle);

	return result;

}

#if HAS_VECTOR3

Matrix4x4 MakeWorldMatrix(const Vector3& translation, const Vector3& scale, const Vector3& rotation) {

	return Matrix4x4{

	    // 1,1([0][0])
	    scale.x * (cosf(rotation.y) * cosf(rotation.z) + sinf(rotation.y) * sinf(rotation.x) * sinf(rotation.z)),

	    // 1,2([0][1])
	    scale.x * (cosf(rotation.x) * sinf(rotation.z)),

	    // 1,3([0][2])
	    scale.x * (cosf(rotation.y) * sinf(rotation.x) * sinf(rotation.z) - sinf(rotation.y) * cosf(rotation.z)),

	    // 1,4([0][3])
	    0.0f,

	    // 2,1([1][0])
	    scale.y * (sinf(rotation.y) * sinf(rotation.x) * cosf(rotation.z) - cosf(rotation.y) * sinf(rotation.z)),

	    // 2,2([1][1])
	    scale.y * (cosf(rotation.x) * cosf(rotation.z)),

	    // 2,3([1][2])
	    scale.y * (sinf(rotation.y) * sinf(rotation.z) + cosf(rotation.y) * sinf(rotation.x) * cosf(rotation.z)),

	    // 2,4([1][3])
	    0.0f,

	    // 3,1([2][0])
	    scale.z * (sinf(rotation.y) * cosf(rotation.x)),

	    // 3,2([2][1])
	    scale.z * (-sinf(rotation.x)),

	    // 3,3([2][2])
	    scale.z * (cosf(rotation.y) * cosf(rotation.x)),

	    // 3,4([2][3])
	    0.0f,

	    // 4,1([3][0])
	    translation.x,

	    // 4,2([3][1])
	    translation.y,

	    // 4,3([3][2])
	    translation.z,

	    // 4,4([3][3])
	    1.0f

	};
}

#endif