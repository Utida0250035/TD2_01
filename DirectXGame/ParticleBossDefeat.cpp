#include "ParticleBossDefeat.h"

#include "Time/DeltaTime.h"
#include "KeMainCamera.h"
#include "KeMatrix3D.h"
#include "Random.h"

#include <numbers>

void Atrum::ParticleBossDefeat::Initialize() { camera_ = KeMainCamera::GetInstance()->camera_; }

void Atrum::ParticleBossDefeat::Update() {

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

		// 正規化
		float length = direction.Length();

		// 生成
		ParticleData* newParticle = new ParticleData;
		newParticle->worldTransform_.Initialize();
		newParticle->objectColor_.Initialize();

		// 初期座標
		newParticle->worldTransform_.translation_.x = initialPosition_.x;
		newParticle->worldTransform_.translation_.y = initialPosition_.y;
		newParticle->worldTransform_.translation_.z = initialPosition_.z;

		// 大きさ
		newParticle->worldTransform_.scale_.x = initialSize_;
		newParticle->worldTransform_.scale_.y = initialSize_;
		newParticle->worldTransform_.scale_.z = initialSize_;

		// 角度
		newParticle->worldTransform_.rotation_.x = Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f);
		newParticle->worldTransform_.rotation_.y = Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f);
		newParticle->worldTransform_.rotation_.z = Random::Create(0.0f, std::numbers::pi_v<float> * 2.0f);

		// 初速指定
		newParticle->velocity_.x = (direction.x / length) * initialSpeed_;
		newParticle->velocity_.y = (direction.y / length) * initialSpeed_;
		newParticle->velocity_.z = (direction.z / length) * initialSpeed_;

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

		// 大きさ減衰
		particle->worldTransform_.scale_.x = Lerp(particle->worldTransform_.scale_.x, 0.0f, 0.05f); 
		particle->worldTransform_.scale_.y = particle->worldTransform_.scale_.x; 
		particle->worldTransform_.scale_.z = particle->worldTransform_.scale_.x; 

		// ベクトル減衰
		particle->velocity_.x = Lerp(particle->velocity_.x, 0.0f, 0.01f); 
		particle->velocity_.y = Lerp(particle->velocity_.y, 0.0f, 0.01f);
		particle->velocity_.z = Lerp(particle->velocity_.z, 0.0f, 0.01f);

		// 座標更新
		particle->worldTransform_.translation_.x += particle->velocity_.x * FrameDeltaTime::GetInstance()->GetDeltaTime();
		particle->worldTransform_.translation_.y += particle->velocity_.y * FrameDeltaTime::GetInstance()->GetDeltaTime();
		particle->worldTransform_.translation_.z += particle->velocity_.z * FrameDeltaTime::GetInstance()->GetDeltaTime();

		particle->worldTransform_.matWorld_ = MakeWorldMatrix(particle->worldTransform_.translation_, particle->worldTransform_.scale_, particle->worldTransform_.rotation_);
		particle->worldTransform_.TransferMatrix();

		particle->alpha_ = Lerp(particle->alpha_, 0.0f, 0.1f);
		particle->objectColor_.SetColor({1.0f, 1.0f, 1.0f, particle->alpha_});

		// 終了
		if (particle->alpha_ <= 0.01f) {
			particle->isFinish_ = true;
		}
	}

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

void Atrum::ParticleBossDefeat::Draw() {

	assert(model_ && "No Model!");

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kNormal, KamataEngine::Model::DepthTestMode::kReadOnly);

	for (auto& particle : particles_) {
		model_->Draw(particle->worldTransform_, *camera_, &particle->objectColor_);
	}

	KamataEngine::Model::PostDraw();
}

void Atrum::ParticleBossDefeat::Finalize() {

	for (auto& particle : particles_) {

		delete particle;
	}
}
