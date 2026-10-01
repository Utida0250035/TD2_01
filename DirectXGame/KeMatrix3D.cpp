#include "KeMatrix3D.h"

KamataEngine::Matrix4x4 MakeWorldMatrix(const KamataEngine::Vector3& translation, const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotation) {

	return KamataEngine::Matrix4x4{

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