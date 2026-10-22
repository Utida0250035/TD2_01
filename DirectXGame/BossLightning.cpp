#include "BossLightning.h"
#include "Math/Transform.h"
#include "System/CmpDrawHitSphere.h"
#include "System/CmpLifeTime.h"
#include "Time/DeltaTime.h"
#include "KeMatrix3D.h"
#include "KeMainCamera.h"
#include "Random.h"
#include <cassert>

namespace Atrum {

void BossLightning::Initialize(const Math::Vector3& emitPos) {

	Math::TransformLH initialTransform{};
	initialTransform.translate = emitPos;

	entity_.reset(new Entity());
	entity_->SetEntityType(EntityType::ENEMY_WEAPON);
	entity_->SetInitialTransform(initialTransform);

	entity_->AddDrawCmp<CmpDrawHitSphere>();

	CmpDrawHitSphere* cmpDrawHitSphere = entity_->GetDrawCmp<CmpDrawHitSphere>();
	cmpDrawHitSphere->SetColor({1.0f, 1.0f, 0.3f, 1.0f});

	entity_->AddUpdCmp<CmpHitSphere>();
	cmpHitSphere_ = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere_->SetIsActive(true);
	cmpHitSphere_->SetSpheres({});

	entity_->AddUpdCmp<CmpLifeTime>();
	CmpLifeTime* cmpLifeTime = entity_->GetUpdCmp<CmpLifeTime>();

	cmpLifeTime->SetLifeTime(0.8f);

	entity_->ResolveDependences();
	entity_->Initialize();

	lightningTransform_.Initialize();
	lightningTransform_.translation_.x = initialTransform.translate.x;
	lightningTransform_.translation_.y = initialTransform.translate.y;
	lightningTransform_.translation_.z = initialTransform.translate.z;
	baseTranslation_ = initialTransform.translate;

	lightningColor_.Initialize();
	lightningColor_.SetColor({1.0f, 1.0f, 0.0f, 1.0f});

	lightningModel_ = KamataEngine::Model::CreateFromOBJ("lightning", false);

	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

void BossLightning::Update() {

	frames_++;

	if (sphereCount_ < sphereMax_) {

		growTimer_ += frameDeltaTime_->GetDeltaTime();

		if (growTimer_ >= growTime_) {

			if (cmpHitSphere_->GetSpheres().size() > 0) {

				cmpHitSphere_->RefSpheres().push_back({cmpHitSphere_->GetSpheres().back().localOffset + offsets_[sphereCount_ % offsets_.size()], kSphereRadius});
			} else {

				cmpHitSphere_->RefSpheres().push_back({{}, kSphereRadius});
			
			}

			assert(cmpHitSphere_->GetSpheres().size() < 1000);

			++sphereCount_;
			growTimer_ = 0.0f;

		}
	}

	entity_->Update();

	attackTimer_ += frameDeltaTime_->GetDeltaTime();

	alpha_ = 1.0f - (attackTimer_ / attackTime_);

	if (frames_ %2 == 0) {
		lightningColor_.SetColor({1.0f, 1.0f, 0.0f, alpha_});
	} else {
		lightningColor_.SetColor({0.0f, 1.0f, 1.0f, alpha_});
	}

	lightningTransform_.translation_.x = baseTranslation_.x + Random::Create(-shakes_, shakes_);
	lightningTransform_.translation_.y = baseTranslation_.y + Random::Create(-shakes_, shakes_);
	lightningTransform_.translation_.z = baseTranslation_.z + Random::Create(-shakes_, shakes_);

	lightningTransform_.scale_.z = growTimer_ / growTime_;
	if (growTimer_ == 0.0f) {
		lightningTransform_.scale_.z = 1.0f;
	}

	lightningTransform_.matWorld_ = MakeWorldMatrix(lightningTransform_.translation_, lightningTransform_.scale_, lightningTransform_.rotation_);
	lightningTransform_.TransferMatrix();

}

void BossLightning::Draw() { 

	if (alpha_ <= 0.1f) {
		return;
	}

	KamataEngine::Model::PreDraw(KamataEngine::Model::CullingMode::kBack, KamataEngine::ModelCommon::BlendMode::kAdd);

	//entity_->Draw();

	lightningModel_->Draw(lightningTransform_, *KeMainCamera::GetInstance()->camera_, &lightningColor_);

	KamataEngine::Model::PostDraw();

}

void BossLightning::Finalize() {

	entity_->Finalize();

}

} // namespace Atrum