#include "Enemy.h"

#include "Math/Lerp.h"
#include "System/EntityFactory.h"

using namespace Atrum;
using namespace Atrum::Math;

void Enemy::Initialize() {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::ENEMY);
	entity_->SetInitialTransform({});

	entity_->AddUpdCmp<CmpHitSphere>();
	entity_->AddUpdCmp<CmpHealthHit>();
	entity_->AddUpdCmp<CmpEnemy>();
	entity_->AddDrawCmp<CmpMesh>();

	CmpEnemy* cmpEnemy = entity_->GetUpdCmp<CmpEnemy>();

	cmpEnemy->SetAnotherOwner(this);

	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("axis"));

	CmpHitSphere* cmpHitSphere = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 3.0f}
    });

	cmpHitSphere->SetEnemy();

	CmpHealthHit* cmpHealthHit = entity_->GetUpdCmp<CmpHealthHit>();
	std::vector<EntityType> hurterTypes;
	hurterTypes.push_back(EntityType::PLAYER_WEAPON);
	cmpHealthHit->SetHurterTypes(hurterTypes);

	entity_->ResolveDependences();
	entity_->Initialize();
}

void Enemy::Update() { entity_->Update(); }

void Enemy::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	KamataEngine::Model::PostDraw();
}

namespace Atrum {
void CmpEnemy::ResolveDependence() {}
void CmpEnemy::Initialize() {}

void CmpEnemy::Update() {

	//--------------------------
	//

	//
	//--------------------------
}

} // namespace Atrum