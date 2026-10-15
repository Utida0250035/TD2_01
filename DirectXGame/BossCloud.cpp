#include "BossCloud.h"

using namespace Atrum;

void BossCloud::Initialize(const Math::Vector3& pos) {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::ENEMY);
	entity_->SetInitialTransform({{3.0f, 3.0f, 3.0f}, {}, pos });

	entity_->AddUpdCmp<CmpHitSphere>();
	entity_->AddUpdCmp<CmpHealthHit>();
	entity_->AddDrawCmp<CmpMesh>();

	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("unitSphere", true));
	cmpMesh->SetColor({0.1f, 0.1f, 0.1f, 1.0f});

	entity_->Update();

	attack_.reset(new BossLightning);
	attack_->Initialize(pos);

	entity_->ResolveDependences();
	entity_->Initialize();

}

void BossCloud::Update() {

	entity_->Update();
	attack_->Update();

	if (!attack_->IsActive()) {
		attack_->Finalize();
		entity_->SetState(Entity::State::Sleep);
	}


}

void BossCloud::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	attack_->Draw();

	KamataEngine::Model::PostDraw();

}