#include "DeathParticles.h"
#include "./Math/AeVector3.h"
#include "./Math/Matrix4x4.h"
#include "Easing.h"
#include "KeVectorUtility.h"
#include "KeMatrix3D.h"
#include <algorithm>

namespace {

namespace I = Atrum::Interpolation;
namespace M = Atrum::Math;

} // namespace

/// <summary>
/// 初期化
/// </summary>
/// <param name="model"> 3Dモデル </param>
/// <param name="textureHandle"> テクスチャハンドル </param>
/// <param name="camera"> カメラ </param>
void DeathParticles::Initialize(KamataEngine::Model* model, const uint32_t& textureHandle, KamataEngine::Camera* camera, const M::Vector3& position) {

	model_ = model;

	camera_ = camera;

	textureHandle_ = textureHandle;

	for (auto& worldTransform : worldTransforms_) {

		worldTransform.Initialize();
		worldTransform.translation_ = ToKamataEngine(position);
	}

	objectColor_.Initialize();

	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
}

/// <summary>
/// 更新
/// </summary>
void DeathParticles::Update(const float& deltaTime) {

	if (isFinished_) {
		// パーティクル終了

		return;
	}

	M::Vector3 velocity{};

	M::Matrix4x4 zRotateMatrix{};

	for (size_t i = 0u; i < kParticlesCount_; ++i) {

		velocity = M::Vector3{kSpeed_, 0.0f, 0.0f};

		zRotateMatrix = M::Matrix4x4::RotateZ(kAngleUnit_ * static_cast<float>(i));

		velocity = zRotateMatrix.Transform(velocity);

		worldTransforms_[i].translation_ += ToKamataEngine(velocity * deltaTime);
	}

	M::Matrix4x4 worldMatrix{};

	for (auto& worldTransform : worldTransforms_) {

		worldMatrix = M::Matrix4x4::World(FromKamataEngine(worldTransform.translation_), FromKamataEngine(worldTransform.scale_), FromKamataEngine(worldTransform.rotation_));

		worldTransform.matWorld_ = ToKamataEngine(worldMatrix);

		worldTransform.TransferMatrix();
	}

	lifeCount_ += deltaTime;

	if (lifeCount_ >= kDuration_) {

		lifeCount_ = kDuration_;

		isFinished_ = true;
	}

	color_.w = I::EaseOutQuart((kDuration_ - lifeCount_) / kDuration_);

	color_.w = std::clamp(color_.w, 0.0f, 1.0f);

	// 色の数値を設定
	objectColor_.SetColor(ToKamataEngine(color_));
}

/// <summary>
/// 描画
/// </summary>
void DeathParticles::Draw() {

	if (isFinished_) {
		// パーティクル終了

		return;
	}

	for (const auto& worldTransform : worldTransforms_) {

		model_->Draw(worldTransform, *camera_, textureHandle_, &objectColor_);
	}
}