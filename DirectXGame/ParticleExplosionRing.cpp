#include "ParticleExplosionRing.h"

#include "KeMainCamera.h"
#include "KeMatrix3D.h"
#include "System/CmpMesh.h"
#include "Time/DeltaTime.h"

#include <iostream>

using namespace Atrum;

void ParticleExplosion::Initialize() {

	camera_ = KeMainCamera::GetInstance()->camera_;

	//-------------------------------------------
	//

	//--------------------
	//

	fire_.worldTransform_.Initialize();
	smoke_.worldTransform_.Initialize();
	ring_.worldTransform_.Initialize();

	//----------

	fire_.worldTransform_.translation_ = initialTranslation_;
	smoke_.worldTransform_.translation_ = initialTranslation_;
	ring_.worldTransform_.translation_ = initialTranslation_;

	//----------

	fire_.worldTransform_.rotation_.x = static_cast<float>(rand() & 360);
	fire_.worldTransform_.rotation_.y = static_cast<float>(rand() & 360);
	fire_.worldTransform_.rotation_.z = static_cast<float>(rand() & 360);

	smoke_.worldTransform_.rotation_.x = static_cast<float>(rand() & 360);
	smoke_.worldTransform_.rotation_.y = static_cast<float>(rand() & 360);
	smoke_.worldTransform_.rotation_.z = static_cast<float>(rand() & 360);

	//
	//--------------------

	fire_.objectColor_.Initialize();
	smoke_.objectColor_.Initialize();
	ring_.objectColor_.Initialize();

	//
	//-------------------------------------------

	// インフォ
	ParticleInfo info{};
	info.lifeTimer_ = 1.0f;
	particleInfo_.push_back(info);
}

void ParticleExplosion::Update() {

	for (auto& info : particleInfo_) {

		info.lifeTimer_ -= 1.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();

		if (info.lifeTimer_ <= 0.0f) {
			isFinish_ = true;
		}
	}

	///--------------------------------------------------------
	///
	///

	#pragma region

	//--------------------------------------
	// 火

	#pragma region fire

	fire_.worldTransform_.scale_.x = Lerp(fire_.worldTransform_.scale_.x, fireTargetSize_, fireSizeLerp_);
	fire_.worldTransform_.scale_.y = fire_.worldTransform_.scale_.x;
	fire_.worldTransform_.scale_.z = fire_.worldTransform_.scale_.x;

	fire_.worldTransform_.matWorld_ = MakeWorldMatrix(fire_.worldTransform_.translation_, fire_.worldTransform_.scale_, fire_.worldTransform_.rotation_);
	fire_.worldTransform_.TransferMatrix();

	//----------------

	fire_.alpha_ = Lerp(fire_.alpha_, 0.0f, fireAlphaLerp_);
	fire_.objectColor_.SetColor({1.0f, 1.0f, 1.0f, fire_.alpha_});

	#pragma endregion

	//
	//--------------------------------------

	//--------------------------------------
	// 煙

	#pragma region smoke

	smoke_.worldTransform_.scale_.x = Lerp(smoke_.worldTransform_.scale_.x, smokeTargetSize_, smokeSizeLerp_);
	smoke_.worldTransform_.scale_.y = smoke_.worldTransform_.scale_.x;
	smoke_.worldTransform_.scale_.z = smoke_.worldTransform_.scale_.x;

	smoke_.worldTransform_.matWorld_ = MakeWorldMatrix(smoke_.worldTransform_.translation_, smoke_.worldTransform_.scale_, smoke_.worldTransform_.rotation_);
	smoke_.worldTransform_.TransferMatrix();

	//----------------

	smoke_.alpha_ = Lerp(smoke_.alpha_, 0.0f, smokeAlphaLerp_);
	smoke_.objectColor_.SetColor({1.0f, 1.0f, 1.0f, smoke_.alpha_});

	#pragma endregion

	//
	//--------------------------------------

	//--------------------------------------
	// リング

	#pragma region ring

	ring_.worldTransform_.scale_.x = Lerp(ring_.worldTransform_.scale_.x, ringTargetSize_, ringSizeLerp_);
	ring_.worldTransform_.scale_.y = ring_.worldTransform_.scale_.x;

	ring_.worldTransform_.matWorld_ = MakeWorldMatrix(ring_.worldTransform_.translation_, ring_.worldTransform_.scale_, ring_.worldTransform_.rotation_);
	ring_.worldTransform_.TransferMatrix();

	//----------------

	ring_.alpha_ = Lerp(ring_.alpha_, 0.0f, ringAlphaLerp_);
	ring_.objectColor_.SetColor({1.0f, 1.0f, 1.0f, ring_.alpha_});

	#pragma endregion

	//
	//--------------------------------------

	#pragma endregion

	///
	///
	///--------------------------------------------------------
}

void ParticleExplosion::Draw() {

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kAdd, KamataEngine::Model::DepthTestMode::kReadOnly);

	fire_.model_->Draw(fire_.worldTransform_, *camera_, &fire_.objectColor_);
	ring_.model_->Draw(ring_.worldTransform_, *camera_, &ring_.objectColor_);

	KamataEngine::Model::PostDraw();

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kNormal, KamataEngine::Model::DepthTestMode::kReadOnly);

	smoke_.model_->Draw(smoke_.worldTransform_, *camera_, &smoke_.objectColor_);

	KamataEngine::Model::PostDraw();
}

void ParticleExplosion::SetModel(KamataEngine::Model* fireModel, KamataEngine::Model* smokeModel, KamataEngine::Model* ringModel) {

	// 引数の欠落を確認
	assert(fireModel && "No FireModel!");
	assert(smokeModel && "No SmokeModel!");
	assert(ringModel && "No RingModel!");

	fire_.model_ = fireModel;
	smoke_.model_ = smokeModel;
	ring_.model_ = ringModel;
}

void ParticleExplosion::Finalize() {}

ParticleExplosion::~ParticleExplosion() { Finalize(); }