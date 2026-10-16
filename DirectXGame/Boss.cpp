#include "Boss.h"

#include "Math/Lerp.h"
#include "Particle/CommandParticle.h"
#include "Random.h"
#include "System/EntityFactory.h"

#include "BossLightning.h"
#include "EnemyEmitter.h"
#include "ParticleBossAppear.h"
#include "ParticleBossDefeat.h"
#include "Time/DeltaTime.h"

#include "Math/Lerp.h"

namespace Atrum {

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
	entity_->AddDrawCmp<CmpMesh>();

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
	cmpHealthHit->SetInitialHealthCount(200);

	entity_->ResolveDependences();
	entity_->Initialize();

	ParticleBossAppear* particleAppear = new ParticleBossAppear;
	particleAppear->SetModel(KamataEngine::Model::CreateFromOBJ("cloudParticle", false));
	particleAppear->Initialize();

	CommandParticle::GetInstance()->Generate("bossAppear", particleAppear);

	ParticleBossDefeat* particleDefeat = new ParticleBossDefeat;
	particleDefeat->SetModel(KamataEngine::Model::CreateFromOBJ("cloudParticle", false));
	particleDefeat->Initialize();
	particleDefeat->SetInitialPosition({});

	CommandParticle::GetInstance()->Generate("bossDefeat", particleDefeat);

	frameDeltaTime_ = FrameDeltaTime::GetInstance();

	healthSprite_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("white1x1.png"), {640.0f - (healthBarWidth_ / 2.0f), 704.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f});
	healthSprite_->SetSize({healthBarWidth_, 16.0f});
}

void Boss::Update() {

	if (CommandParticle::GetInstance()->IsExist("bossAppear"))
		return;

	entity_->Update();

	summonLoopTimer_ += frameDeltaTime_->GetDeltaTime();

	if (summonLoopTimer_ >= summonLoopTime_) {

		summonLoopTimer_ = 0.0f;

		EnemyEmitter::Emit(enemies_, entity_->GetWorldPosition());
	}

	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	// 取り巻き削除
	std::erase_if(enemies_, [](const std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			enemy->GetEntity()->Finalize();
			return true;
		}
		return false;
	});

	attackLoopTimer_ += frameDeltaTime_->GetDeltaTime();

	if (attackLoopTimer_ >= attackLoopTime_) {

		attackLoopTimer_ = 0.0f;

		for (size_t i = 0; i < 2; ++i) {
			lightningAttacks_.emplace_back(std::make_unique<Atrum::BossLightning>());
			lightningAttacks_.back()->Initialize(entity_->GetWorldPosition() + lightningAttackOffsets_[i]);
		}
	}

	for (auto& lightning : lightningAttacks_) {

		if (!lightning)
			continue;

		lightning->Update();
		if (lightning && !lightning->IsActive()) {
			lightning->Finalize();
		}
	}

	std::erase_if(lightningAttacks_, [](const auto& lightning) {
		if (!lightning || !lightning->IsActive()) {
			return true;
		}

		return false;
	});

	// 体力
	CmpHealthHit* cmpHealthHit = entity_->GetUpdCmp<CmpHealthHit>();

	float currentBarWidth = healthSprite_->GetSize().x;
	float endBarWidth = (static_cast<float>(cmpHealthHit->GetHealthCount()) / static_cast<float>(cmpHealthHit->GetInitialHealthCount())) * healthBarWidth_;

	healthSprite_->SetSize({Interpolation::Lerp(currentBarWidth, endBarWidth, 0.2f), 16.0f});
}

void Boss::DrawUI() {

	KamataEngine::Sprite::PreDraw();

	healthSprite_->Draw();

	KamataEngine::Sprite::PostDraw();
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

	for (auto& lightning : lightningAttacks_) {

		if (lightning)
			lightning->Draw();
	}

	CommandParticle::GetInstance()->Draw("bossAppear");
}

} // namespace Atrum