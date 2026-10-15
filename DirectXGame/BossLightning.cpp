#include "BossLightning.h"
#include "Math/Transform.h"
#include "System/CmpDrawHitSphere.h"
#include "System/CmpLifeTime.h"
#include "Time/DeltaTime.h"
#include <KamataEngine.h>

namespace Atrum {

void BossLightning::Initialize(const Math::Vector3& emitPos) {

	Math::TransformLH initialTransform{};
	initialTransform.translate = emitPos;

	entity_.reset(new Entity);
	entity_->SetEntityType(EntityType::ENEMY_WEAPON);
	entity_->SetInitialTransform(initialTransform);

	entity_->AddDrawCmp<CmpDrawHitSphere>();

	CmpDrawHitSphere* cmpDrawHitSphere = entity_->GetDrawCmp<CmpDrawHitSphere>();
	cmpDrawHitSphere->SetColor({0.75f, 0.75f, 0.1f, 1.0f});

	entity_->AddUpdCmp<CmpHitSphere>();
	cmpHitSphere_ = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere_->SetIsActive(true);

	entity_->AddUpdCmp<CmpLifeTime>();
	CmpLifeTime* cmpLifeTime = entity_->GetUpdCmp<CmpLifeTime>();

	cmpLifeTime->SetLifeTime(1.0f);

	entity_->ResolveDependences();
	entity_->Initialize();

	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

void BossLightning::Update() {

	if (sphereCount_ < sphereMax_) {

		growTimer_ += frameDeltaTime_->GetDeltaTime();

		if (growTimer_ >= growTime_) {

			std::vector<HitSphere> hitSpheres = cmpHitSphere_->GetSpheres();

			if (hitSpheres.size() > 0) {

				hitSpheres.push_back({hitSpheres.back().localOffset + offsets_[sphereCount_ % 4], kSphereRadius});
			} else {

				hitSpheres.push_back({{}, kSphereRadius});
			
			}

			cmpHitSphere_->SetSpheres(hitSpheres);

			++sphereCount_;
			growTimer_ = 0.0f;

		}
	}

	entity_->Update();

}

void BossLightning::Draw() { 

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	KamataEngine::Model::PostDraw();

}

bool BossLightning::IsActive() { return entity_->GetState() == Entity::State::Active; }

void BossLightning::Finalize() {

	entity_->Finalize();

}

} // namespace Atrum