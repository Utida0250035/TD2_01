#include "BossLightning.h"
#include "Math/Transform.h"
#include "System/CmpDrawHitSphere.h"
#include "System/CmpLifeTime.h"
#include "Time/DeltaTime.h"
#include <KamataEngine.h>
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

	cmpLifeTime->SetLifeTime(1.0f);

	entity_->ResolveDependences();
	entity_->Initialize();

	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

void BossLightning::Update() {

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

}

void BossLightning::Draw() { 

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	KamataEngine::Model::PostDraw();

}

void BossLightning::Finalize() {

	entity_->Finalize();

}

} // namespace Atrum