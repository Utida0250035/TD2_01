#include "ParticleExplosionRing.h"

#include "KeMainCamera.h"
#include "KeMatrix3D.h"
#include "System/CmpMesh.h"
#include "Time/DeltaTime.h"

#include "Random.h"
#include <numbers>
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
	collisionRing_.worldTransform_.Initialize();

	//----------

	fire_.worldTransform_.translation_ = initialTranslation_;
	smoke_.worldTransform_.translation_ = initialTranslation_;
	ring_.worldTransform_.translation_ = initialTranslation_;
	collisionRing_.worldTransform_.translation_ = initialTranslation_;

	//----------

	fire_.worldTransform_.rotation_.x = static_cast<float>(Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f));
	fire_.worldTransform_.rotation_.y = static_cast<float>(Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f));
	fire_.worldTransform_.rotation_.z = static_cast<float>(Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f));

	smoke_.worldTransform_.rotation_.x = static_cast<float>(Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f));
	smoke_.worldTransform_.rotation_.y = static_cast<float>(Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f));
	smoke_.worldTransform_.rotation_.z = static_cast<float>(Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f));

	//
	//--------------------

	fire_.objectColor_.Initialize();
	smoke_.objectColor_.Initialize();
	ring_.objectColor_.Initialize();
	collisionRing_.objectColor_.Initialize();

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

	//--------------------------------------
	// 判定可視化リング

	#pragma region collisionRing

	collisionRing_.worldTransform_.scale_.x = Lerp(collisionRing_.worldTransform_.scale_.x, collisionRingTargetSize_, collisionRingSizeLerp_);
	collisionRing_.worldTransform_.scale_.y = collisionRing_.worldTransform_.scale_.x;

	collisionRing_.worldTransform_.matWorld_ = MakeWorldMatrix(collisionRing_.worldTransform_.translation_, collisionRing_.worldTransform_.scale_, collisionRing_.worldTransform_.rotation_);
	collisionRing_.worldTransform_.TransferMatrix();

	//----------------

	collisionRing_.alpha_ = Lerp(collisionRing_.alpha_, 0.0f, collisionRingAlphaLerp_);
	collisionRing_.objectColor_.SetColor({1.0f, 1.0f, 1.0f, collisionRing_.alpha_});

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
	collisionRing_.model_->Draw(collisionRing_.worldTransform_, *camera_, &collisionRing_.objectColor_);

	KamataEngine::Model::PostDraw();

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kNormal, KamataEngine::Model::DepthTestMode::kReadOnly);

	smoke_.model_->Draw(smoke_.worldTransform_, *camera_, &smoke_.objectColor_);

	KamataEngine::Model::PostDraw();
}

void ParticleExplosion::SetModel(KamataEngine::Model* fireModel, KamataEngine::Model* smokeModel, KamataEngine::Model* ringModel, KamataEngine::Model* collisionRingModel) {

	// 引数の欠落を確認
	assert(fireModel && "No FireModel!");
	assert(smokeModel && "No SmokeModel!");
	assert(ringModel && "No RingModel!");

	fire_.model_ = fireModel;
	smoke_.model_ = smokeModel;
	ring_.model_ = ringModel;
	collisionRing_.model_ = collisionRingModel;
}

void ParticleExplosion::Finalize() {}

ParticleExplosion::~ParticleExplosion() { Finalize(); }