#include "BossCloud.h"

#include "Time/DeltaTime.h"
#include "Math/Lerp.h"

using namespace Atrum;

void BossCloud::Initialize(const Math::Vector3& pos) {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::ENEMY);
	entity_->SetInitialTransform({{5.0f, 3.0f, 3.0f}, {}, pos });

	entity_->AddUpdCmp<CmpHitSphere>();
	entity_->AddUpdCmp<CmpHealthHit>();
	entity_->AddDrawCmp<CmpMesh>();

	cmpMesh_ = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh_->SetModel(KamataEngine::Model::CreateFromOBJ("unitSphere", true));
	cmpMesh_->SetColor({0.1f, 0.1f, 0.1f, 0.0f});

	entity_->Update();

	entity_->ResolveDependences();
	entity_->Initialize();

}

void BossCloud::Update() {


	cmpMesh_->SetColor({0.1f, 0.1f, 0.1f, alpha_});

	timer_ += FrameDeltaTime::GetInstance()->GetDeltaTime();

	if (timer_ >= 1.0f) {
		alpha_ = Interpolation::Lerp(alpha_, 0.0f, 0.2f);
	} else {
		alpha_ = Interpolation::Lerp(alpha_, 1.0f, 0.2f);
	}

	if (timer_ >= 0.5f) {
		if (attack_.get() == nullptr) {
			attack_.reset(new BossLightning);
			attack_->Initialize(entity_->GetWorldPosition());
		}
	}

	entity_->Update();

	if (attack_.get()) {

		attack_->Update();

		if (!attack_->IsActive()) {
			attack_->Finalize();
			entity_->SetState(Entity::State::Sleep);
			isActive_ = false;
		}

	}

	


}

void BossCloud::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	if (attack_) {
		attack_->Draw();
	}

	KamataEngine::Model::PostDraw();

}