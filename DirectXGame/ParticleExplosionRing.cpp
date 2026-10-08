#include "ParticleExplosionRing.h"

#include "System/CmpMesh.h"
#include "Time/DeltaTime.h"
#include "KeMainCamera.h"
#include "KeMatrix3D.h"

#include <iostream>

using namespace Atrum;

void ParticleExplosionRing::Initialize() {

	camera_ = KeMainCamera::GetInstance()->camera_;

	worldTransform_.Initialize();
	worldTransform_.translation_ = initialTranslation_;

	objectColor_.Initialize();

	// インフォ
	ParticleInfo info{};
	info.lifeTimer_ = 1.0f;
	particleInfo_.push_back(info);
}

void ParticleExplosionRing::Update() {

	for (auto& info : particleInfo_) {

		info.lifeTimer_ -= 1.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();

		if (info.lifeTimer_ <= 0.0f) {
			isFinish_ = true;
		}

	}

	worldTransform_.scale_.x = Lerp(worldTransform_.scale_.x, endSize_, sizeLerp_);
	worldTransform_.scale_.y = worldTransform_.scale_.x;
	worldTransform_.scale_.z = worldTransform_.scale_.x;

	worldTransform_.matWorld_ = MakeWorldMatrix(worldTransform_.translation_, worldTransform_.scale_, worldTransform_.rotation_);
	worldTransform_.TransferMatrix();

	alpha_ = Lerp(alpha_, 0.0f, alphaLerp_);
	objectColor_.SetColor({1.0f, 1.0f, 1.0f, alpha_});

	if (alpha_ <= 0.01f) {
		isFinish_ = true;
	}

}

void ParticleExplosionRing::Draw() {

	if (model_ == nullptr) {
		return;
	}

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kAdd, KamataEngine::Model::DepthTestMode::kReadOnly);

	model_->Draw(worldTransform_, *camera_, &objectColor_);

	KamataEngine::Model::PostDraw();
}

void ParticleExplosionRing::Finalize() {}

ParticleExplosionRing::~ParticleExplosionRing() { Finalize(); }