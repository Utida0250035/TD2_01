#pragma once

#include "Math/AeVector3.h"
#include <KamataEngine.h>

struct AABB {
	Atrum::Math::Vector3 min{};
	Atrum::Math::Vector3 max{};
};

inline constexpr bool IsCollisionAABB(const AABB& aabb1, const AABB& aabb2) {

	if (aabb1.min.x < aabb2.max.x && aabb1.max.x > aabb2.min.x) {
	
		if (aabb1.min.y < aabb2.max.y && aabb1.max.y > aabb2.min.y) {
		
			if (aabb1.min.z < aabb2.max.z && aabb1.max.z > aabb2.min.z) {

				return true;

			}
		
		}

	}

	return false;

}