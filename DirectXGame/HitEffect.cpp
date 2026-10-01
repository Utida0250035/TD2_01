#include "HitEffect.h"
#include "./Math/Lerp.h"
#include "Easing.h"

#include "KeMatrix3D.h"

#include <cassert>
#include <stdlib.h>

namespace {

namespace I = Atrum::Interpolation;
namespace M = Atrum::Math;

}

KamataEngine::Model* HitEffect::model_ = nullptr;
KamataEngine::Camera* HitEffect::camera_ = nullptr;

HitEffect* HitEffect::Create(const M::Vector3& position, const float lifeTime) {

	HitEffect* instance = new HitEffect();

	assert(instance);

	instance->Initialize(position, lifeTime);

	return instance;
}

void HitEffect::Initialize(const M::Vector3& position, const float lifeTime) {

	lifeCount_ = 0.0f;

	lifeTime_ = lifeTime;

	position_ = position;

	for (auto& worldTransform : worldTransforms_) {

		worldTransform.Initialize();
	}

	objectColor_.Initialize();

	for (auto& angle : slashAngle_) {

		angle = static_cast<float>(rand() % 628) * 0.01f;
	}
}

void HitEffect::Update(const float deltaTime) {

	if (lifeCount_ >= lifeTime_) {

		isFinish_ = true;

	} else {

		lifeCount_ += deltaTime;
	}

	float scale = I::Lerp(I::EaseOutCirc(lifeCount_ / lifeTime_), 0.0f, 1.0f);

	objectColor_.SetColor({1.0f, 1.0f, 1.0f, I::EaseOutCirc(1.0f - lifeCount_ / lifeTime_)});

	worldTransforms_[0].translation_ = {position_.x, position_.y, position_.z};
	worldTransforms_[0].rotation_ = {};
	worldTransforms_[0].scale_ = {scale, scale, scale};
	UpdateWorldTransform(worldTransforms_[0]);

	for (size_t i = 0; i < 2; i++) {

		worldTransforms_[i + 1].translation_ = {position_.x, position_.y, position_.z};

		worldTransforms_[i + 1].scale_ = {0.125f * scale, 1.25f * scale, scale};

		worldTransforms_[i + 1].rotation_ = {0.0f, 0.0f, slashAngle_[i]};

		UpdateWorldTransform(worldTransforms_[i + 1]);
	}
}

void HitEffect::Draw() {

	if (isFinish_) {
		return;
	}

	for (const auto& worldTransform : worldTransforms_) {

		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
}