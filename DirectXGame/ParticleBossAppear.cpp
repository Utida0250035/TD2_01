#include "ParticleBossAppear.h"

#include "Time/DeltaTime.h"
#include "KeMainCamera.h"
#include "KeMatrix3D.h"
#include "Random.h"

#include <numbers>

void Atrum::ParticleBossAppear::Initialize() {

	camera_ = KeMainCamera::GetInstance()->camera_;

}

void Atrum::ParticleBossAppear::Update() {

	///-------------------------------------
	/// 新規生成
	///
	
	#pragma region create

	if (canCreate_) {
	
		// 方向
		Math::Vector3 direction = {};
		direction.x = Random::Create(-1.0f, 1.0f);
		direction.y = Random::Create(-1.0f, 1.0f);
		direction.z = Random::Create(-1.0f, 1.0f);

		float length = direction.Length();

		ParticleData* newParticle = new ParticleData;
		newParticle->worldTransform_.Initialize();
		newParticle->objectColor_.Initialize();

		newParticle->worldTransform_.translation_.x = (direction.x / length) * radius_;
		newParticle->worldTransform_.translation_.y = (direction.y / length) * radius_;
		newParticle->worldTransform_.translation_.z = (direction.z / length) * radius_;

		newParticle->worldTransform_.rotation_.x = Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f);
		newParticle->worldTransform_.rotation_.y = Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f);
		newParticle->worldTransform_.rotation_.z = Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f);

		newParticle->acceleration_ = -direction.Normalized() * 20.0f;

		particles_.push_back(newParticle);

	}

	#pragma endregion

	///
	///
	///-------------------------------------

	///-------------------------------------
	/// 更新
	///

	for (auto& particle : particles_) {

		particle->velocity_ += particle->acceleration_ * FrameDeltaTime::GetInstance()->GetDeltaTime();

		particle->worldTransform_.translation_.x += particle->velocity_.x * FrameDeltaTime::GetInstance()->GetDeltaTime();
		particle->worldTransform_.translation_.y += particle->velocity_.y * FrameDeltaTime::GetInstance()->GetDeltaTime();
		particle->worldTransform_.translation_.z += particle->velocity_.z * FrameDeltaTime::GetInstance()->GetDeltaTime();

		particle->worldTransform_.matWorld_ = MakeWorldMatrix(particle->worldTransform_.translation_, particle->worldTransform_.scale_, particle->worldTransform_.rotation_);
		particle->worldTransform_.TransferMatrix();

		float translationToDistance = sqrtf(
		    powf(particle->worldTransform_.translation_.x - targetPosition_.x, 2.0f) +
			powf(particle->worldTransform_.translation_.y - targetPosition_.y, 2.0f) +
		    powf(particle->worldTransform_.translation_.z - targetPosition_.z, 2.0f));

		particle->alpha_ = Lerp(particle->alpha_, 1.0f, 0.05f);
		particle->objectColor_.SetColor({1.0f, 1.0f, 1.0f, particle->alpha_});

		// 終了
		if (translationToDistance <= 0.9f) {
			particle->isFinish_ = true;
		}

	}

	//------------------
	// 生成タイマー

	timer_ -= 1.0f * FrameDeltaTime::GetInstance()->GetDeltaTime();

	if (timer_ <= 0.0f) {
		canCreate_ = false;
	}

	//
	//------------------

	///
	///
	///-------------------------------------

	///-------------------------------------
	/// 削除
	///

	particles_.remove_if([](auto* particle) {
		if (particle->isFinish_) {
			delete particle;
			return true;
		}
		return false;
	});

	///
	///
	///-------------------------------------

	///-------------------------------------
	/// 終了
	///

	// 生成出来ない状態で、particles_が空であると判断したら削除申請を行う
	if (!canCreate_) {
		if (particles_.empty()) {
			isFinish_ = true;
		}
	}

	///
	///
	///-------------------------------------

}

void Atrum::ParticleBossAppear::Draw() {

	assert(model_ && "No Model!");

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kNormal, KamataEngine::Model::DepthTestMode::kReadOnly);

	for (auto& particle : particles_) {
		model_->Draw(particle->worldTransform_, *camera_, &particle->objectColor_);
	}

	KamataEngine::Model::PostDraw();

}

void Atrum::ParticleBossAppear::Finalize() {

	for (auto &particle : particles_) {

		delete particle;

	}

}
