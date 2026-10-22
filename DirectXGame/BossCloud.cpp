#include "BossCloud.h"

#include "Time/DeltaTime.h"
#include "Math/Lerp.h"
#include "../SEPlayer.h"

using namespace Atrum;

void BossCloud::Initialize(const Math::Vector3& pos) {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::ENEMY);
	entity_->SetInitialTransform({{5.0f, 3.0f, 3.0f}, {}, pos });

	entity_->AddUpdCmp<CmpHitSphere>();
	entity_->AddUpdCmp<CmpHealthHit>();
	entity_->AddDrawCmp<CmpMesh>();

	cmpMesh_ = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh_->SetModel(KamataEngine::Model::CreateFromOBJ("fallCloud", true));
	cmpMesh_->SetColor({0.1f, 0.1f, 0.1f, 0.0f});

	entity_->Update();

	entity_->ResolveDependences();
	entity_->Initialize();

}

void BossCloud::Update() {


	cmpMesh_->SetColor({0.1f, 0.1f, 0.1f, alpha_});

	timer_ += FrameDeltaTime::GetInstance()->GetDeltaTime();

	if (isDisappear_) {
		alpha_ = Interpolation::Lerp(alpha_, 0.0f, 0.1f);
	} else {
		alpha_ = Interpolation::Lerp(alpha_, 1.0f, 0.2f);
	}

	if (timer_ >= attackStartTime_) {
		if (attack_ == nullptr) {
			attack_.reset(new BossLightning());
			attack_->Initialize(entity_->GetWorldPosition());
			SEPlayer::GetInstance()->Play("./Resources/seBossAttackLightning.mp3");
		}
	}

	entity_->Update();

	if (attack_) {

		attack_->Update();

		if (!attack_->IsActive()) {
			attack_->Finalize();
			isDisappear_ = true;
		}

	}

	
	if (isDisappear_) {
		if (alpha_ <= 0.01f) {
			entity_->Finalize();
			entity_->SetState(Entity::State::Sleep);
			isActive_ = false;
		}
	}

}

void BossCloud::Draw() {

	if (attack_) {
		attack_->Draw();
	}

	KamataEngine::Model::PreDraw(KamataEngine::ModelCommon::CullingMode::kBack, KamataEngine::Model::BlendMode::kNormal, KamataEngine::Model::DepthTestMode::kReadOnly);

	entity_->Draw();

	KamataEngine::Model::PostDraw();

}