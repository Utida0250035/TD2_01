#pragma once

#include "../Math/Matrix4x4.h"
#include "../Math/Transform.h"
#include "KamataEngine.h"
#include <cmath>

KamataEngine::Matrix4x4 MakeWorldMatrix(const KamataEngine::Vector3& translation, const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotation);

inline void UpdateWorldTransform(KamataEngine::WorldTransform& worldTransform) {

	worldTransform.matWorld_ = MakeWorldMatrix(worldTransform.translation_, worldTransform.scale_, worldTransform.rotation_);
	worldTransform.TransferMatrix();
}

inline constexpr KamataEngine::Matrix4x4 ToKamataEngine(const Atrum::Math::Matrix4x4& mat) {

	KamataEngine::Matrix4x4 matrix{};

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {

			matrix.m[i][j] = mat.m[i][j];
		}
	}

	return matrix;
}

inline constexpr Atrum::Math::Matrix4x4 FromKamataEngine(const KamataEngine::Matrix4x4& mat) {

	Atrum::Math::Matrix4x4 matrix{};

	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 4; ++j) {

			matrix.m[i][j] = mat.m[i][j];
		}
	}

	return matrix;
}

inline void UpdateWorldTransform(KamataEngine::WorldTransform& worldTransform, const Atrum::Math::Matrix4x4& worldMatrix) {
	worldTransform.matWorld_ = ToKamataEngine(worldMatrix);
	worldTransform.TransferMatrix();
}