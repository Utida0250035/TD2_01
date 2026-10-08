#include "Boss.h"

#include "Math/Lerp.h"
#include "System/EntityFactory.h"
#include "Particle/CommandParticle.h"

#include "ParticleBossAppear.h"

using namespace Atrum;
using namespace Atrum::Math;

void Boss::Initialize() {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::ENEMY);
	entity_->SetInitialTransform({});

	entity_->AddUpdCmp<CmpHitSphere>();
	entity_->AddUpdCmp<CmpHealthHit>();
	entity_->AddUpdCmp<CmpBoss>();
	entity_->AddDrawCmp<CmpMesh>();

	CmpBoss* cmpBoss = entity_->GetUpdCmp<CmpBoss>();

	cmpBoss->SetAnotherOwner(this);

	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::Create());

	CmpHitSphere* cmpHitSphere = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 5.0f}
    });

	CmpHealthHit* cmpHealthHit = entity_->GetUpdCmp<CmpHealthHit>();
	std::vector<EntityType> hurterTypes;
	hurterTypes.push_back(EntityType::PLAYER_WEAPON);
	cmpHealthHit->SetHurterTypes(hurterTypes);

	entity_->ResolveDependences();
	entity_->Initialize();

	ParticleBossAppear* particle = new ParticleBossAppear;
	particle->SetModel(KamataEngine::Model::CreateFromOBJ("cloudParticle", false));
	particle->Initialize();

	CommandParticle::GetInstance()->Generate("bossAppear", particle);

}

void Boss::Update() {

	entity_->Update();

}

void Boss::Draw() {

	KamataEngine::Model::PreDraw();

	if (entity_->GetUpdCmp<CmpHealthHit>()->GetHealthCount() > 0) {
		entity_->Draw();
	}

	KamataEngine::Model::PostDraw();

	CommandParticle::GetInstance()->Draw("bossAppear");

}

namespace Atrum {
void CmpBoss::ResolveDependence() {}
void CmpBoss::Initialize() {

}

void CmpBoss::Update() {

	//--------------------------
	// 



	//
	//--------------------------
}

} // namespace Atrum