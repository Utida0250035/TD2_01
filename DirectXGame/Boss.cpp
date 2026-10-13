#include "Boss.h"

#include "Math/Lerp.h"
#include "System/EntityFactory.h"
#include "Particle/CommandParticle.h"
#include "Random.h"

#include "ParticleBossAppear.h"

using namespace Atrum;
using namespace Atrum::Math;

Boss::~Boss() {
	for (auto& enemy : enemies_) {
		enemy->GetEntity()->Finalize();
	}
}

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

	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	// 取り巻き削除
	std::erase_if(enemies_, [](const std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetEntity()->GetUpdCmp<CmpHealthHit>()->GetHealthCount() <= 0) {
			enemy->GetEntity()->Finalize();
			return true;
		}
		return false;
	});
}

void Boss::Draw() {

	KamataEngine::Model::PreDraw();

	if (entity_->GetUpdCmp<CmpHealthHit>()->GetHealthCount() > 0) {
		entity_->Draw();
	}

	KamataEngine::Model::PostDraw();
	
	for (auto& enemy : enemies_) {
		enemy->Draw();
	}

	CommandParticle::GetInstance()->Draw("bossAppear");

}

namespace Atrum {
void CmpBoss::ResolveDependence() {}
void CmpBoss::Initialize() {

}

void CmpBoss::Update() {

	//--------------------------
	// 取り巻き召喚テスト

	KamataEngine::Input* input_ = KamataEngine::Input::GetInstance();
	if (input_->TriggerKey(DIK_T)) {
		std::unique_ptr<Enemy> enemy;
		enemy = std::make_unique<Enemy>();
		enemy->Initialize();
		enemy->GetEntity()->RefTransform().translate.x = Random::Create(-30.0f, 30.0f);
		enemy->GetEntity()->RefTransform().translate.y = Random::Create(-20.0f, 20.0f);
		anotherOwner_->GetEnemies().push_back(std::move(enemy));
	}


	//
	//--------------------------
}

} // namespace Atrum