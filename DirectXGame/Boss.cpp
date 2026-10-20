#include "Boss.h"

#include "../SEPlayer.h"
#include "Math/Lerp.h"
#include "Particle/CommandParticle.h"
#include "Random.h"
#include "System/EntityFactory.h"

#include "BossLightning.h"
#include "EnemyEmitter.h"
#include "ParticleBossAppear.h"
#include "ParticleBossDefeat.h"
#include "Time/DeltaTime.h"

#include <cassert>

using namespace Atrum::Math;

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
	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("boss"));

	CmpHitSphere* cmpHitSphere = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 6.0f}
    });

	CmpHealthHit* cmpHealthHit = entity_->GetUpdCmp<CmpHealthHit>();
	std::vector<EntityType> hurterTypes;
	hurterTypes.push_back(EntityType::PLAYER_WEAPON);
	cmpHealthHit->SetHurterTypes(hurterTypes);
	cmpHealthHit->SetInitialHealthCount(80);

	entity_->ResolveDependences();
	entity_->Initialize();

	ParticleBossAppear* particleAppear = new ParticleBossAppear;
	particleAppear->SetModel(KamataEngine::Model::CreateFromOBJ("cloudParticle", false));
	particleAppear->Initialize();

	CommandParticle::GetInstance()->Generate("bossAppear", particleAppear);

	frameDeltaTime_ = FrameDeltaTime::GetInstance();

	// 座標追加
	movementPositions_ = {
	    {20.0f,  15.0f,  0.0f},
        {0.0f,   15.0f,  0.0f},
        {-20.0f, 15.0f,  0.0f},
        {20.0f,  -10.0f, 0.0f},
        {0.0f,   -10.0f, 0.0f},
        {-20.0f, -10.0f, 0.0f},
	};

	currentTargetPosition_ = movementPositions_[Random::Create(0, static_cast<int>(movementPositions_.size() - 1))];

	healthSprite_ = KamataEngine::Sprite::Create(KamataEngine::TextureManager::Load("white1x1.png"), {640.0f - (healthBarWidth_ / 2.0f), 704.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f});
	healthSprite_->SetSize({healthBarWidth_, 16.0f});

}

void Boss::Update() {

	if (CommandParticle::GetInstance()->IsExist("bossAppear")) {

		appearTimer_ += frameDeltaTime_->GetDeltaTime();

		if (!isSpawnSoundPlayed_) {
			isSpawnSoundPlayed_ = true;
			SEPlayer::GetInstance()->Play("./Resources/seBossSpawn.mp3");
		}

		if (!isScreamSoundPlayed_) {
			if (appearTimer_ >= 3.0f) {

				isScreamSoundPlayed_ = true;
				SEPlayer::GetInstance()->Play("./Resources/seBossScream.mp3");
			}
		}

		return;
	}

	// 死亡演出移行
	if (isDeath_) {

		deathTimer_ += frameDeltaTime_->GetDeltaTime();

		if (deathTimer_ >= deathTime_) {
			isDeathFinished_ = true;
		}
	}

	// 行動タイマー
	actionTimer_ += frameDeltaTime_->GetDeltaTime();

	// 行動別の更新
	if (!isDeath_) {
		switch (currentAction_) {

		case Move:

			entity_->RefTransform().translate = Interpolation::Lerp(entity_->RefTransform().translate, currentTargetPosition_, 0.05f);

			// 移行
			if (actionTimer_ >= moveActionMaxTime_) {

				actionTimer_ = 0.0f;
				attackLimitTime_ = Random::Create(attackMinTime_, attackMinTime_);
				cloudLoopTimer_ = cloudLoopTime_;
				summonLoopTimer_ = summonLoopTime_;

				currentAction_ = static_cast<Actions>(Random::Create(static_cast<int>(EnemySummon), static_cast<int>(Count) - 1));
			}

			break;

		case EnemySummon:

			// 取り巻き召喚
			summonLoopTimer_ += frameDeltaTime_->GetDeltaTime();

			if (summonLoopTimer_ >= summonLoopTime_) {
				summonLoopTimer_ = 0.0f;
				EnemyEmitter::Emit(enemies_, entity_->GetWorldPosition());
			}

			if (actionTimer_ >= attackLimitTime_) {

				actionTimer_ = 0.0f;

				currentTargetPosition_ = movementPositions_[Random::Create(0, static_cast<int>(movementPositions_.size() - 1))];
				currentAction_ = Move;
			}

			break;

		case CloudSummon:

			// 雲召喚
			cloudLoopTimer_ += frameDeltaTime_->GetDeltaTime();

			if (cloudLoopTimer_ >= cloudLoopTime_) {

				cloudLoopTimer_ = 0.0f;

				for (size_t i = 0; i < 2; ++i) {
					clouds_.emplace_back(std::make_unique<Atrum::BossCloud>());
					clouds_.back()->Initialize(entity_->GetWorldPosition() + lightningAttackOffsets_[i]);
				}
			}

			if (actionTimer_ >= attackLimitTime_) {

				actionTimer_ = 0.0f;

				currentTargetPosition_ = movementPositions_[Random::Create(0, static_cast<int>(movementPositions_.size() - 1))];
				currentAction_ = Move;
			}

			break;

		case AreaSummon:

			if (!isAreaAttacked_) {

				isAreaAttacked_ = true;

				SEPlayer::GetInstance()->Play("./Resources/seAreaWarning.mp3");

				switch (Random::Create(0, 2)) {

					case 0:

						areaAttacks_.emplace_back(std::make_unique<Atrum::AreaAttack>());
					    areaAttacks_.back()->Initialize({-26.5f, 0.0f, 0.0f});
					    areaAttacks_.emplace_back(std::make_unique<Atrum::AreaAttack>());
					    areaAttacks_.back()->Initialize({26.5f, 0.0f, 0.0f});

						break;

					case 1:

					    areaAttacks_.emplace_back(std::make_unique<Atrum::AreaAttack>());
					    areaAttacks_.back()->Initialize({-15.0f, 0.0f, 0.0f});
					    areaAttacks_.emplace_back(std::make_unique<Atrum::AreaAttack>());
					    areaAttacks_.back()->Initialize({15.0f, 0.0f, 0.0f});

					    break;

					case 2:

						 areaAttacks_.emplace_back(std::make_unique<Atrum::AreaAttack>());
						 areaAttacks_.back()->Initialize({0.0f, 0.0f, 0.0f});

					    break;

				}

			}

			if (actionTimer_ >= 3.0f) {

				actionTimer_ = 0.0f;
				isAreaAttacked_ = false;

				currentTargetPosition_ = movementPositions_[Random::Create(0, static_cast<int>(movementPositions_.size() - 1))];
				currentAction_ = Move;
			}

			break;

		default:

			assert(false && "currentAction_ has something wrong");

			break;
		}
	}

	// 本体更新
	entity_->Update();

	// 死亡確認
	if (!isDeath_) {

		CmpHealthHit* cmpHealthHit = entity_->GetUpdCmp<CmpHealthHit>();

		if (cmpHealthHit->GetHealthCount() <= 0) {

			isDeath_ = true;

			SEPlayer::GetInstance()->Play("./Resources/seBossDeath.mp3");

			ParticleBossDefeat* particleDefeat = new ParticleBossDefeat;
			particleDefeat->SetModel(KamataEngine::Model::CreateFromOBJ("cloudParticle", false));
			particleDefeat->SetInitialPosition(entity_->GetWorldPosition());
			particleDefeat->Initialize();

			CommandParticle::GetInstance()->Generate("bossDefeat", particleDefeat);
		}
	}

#pragma region

	//-------------------------------
	//

	// 取り巻き更新
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

	//
	//-------------------------------

	//-------------------------------
	//

	// 曇更新
	for (auto& cloud : clouds_) {
		cloud->Update();
	}

	// 雲消去
	std::erase_if(clouds_, [](const auto& cloud) {
		if (!cloud || !cloud->IsActive()) {
			return true;
		}

		return false;
	});

	//
	//-------------------------------

	//-------------------------------
	//

	// 範囲攻撃更新
	for (auto& attack : areaAttacks_) {
		attack->Update();
	}

	// 範囲攻撃消去
	std::erase_if(areaAttacks_, [](const auto& attack) {
		if (!attack || !attack->IsActive()) {
			return true;
		}

		return false;
	});

	//
	//-------------------------------

#pragma endregion

	// 体力
	CmpHealthHit* cmpHealthHit = entity_->GetUpdCmp<CmpHealthHit>();

	float currentBarWidth = healthSprite_->GetSize().x;
	float endBarWidth = (static_cast<float>(cmpHealthHit->GetHealthCount()) / static_cast<float>(cmpHealthHit->GetInitialHealthCount())) * healthBarWidth_;

	healthSprite_->SetSize({Interpolation::Lerp(currentBarWidth, endBarWidth, 0.2f), 16.0f});

	// ダメージリアクション
	if (damageReactionTimer_ > 0) {
		damageReactionTimer_--;
	} else {
		entity_->GetDrawCmp<CmpMesh>()->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}

	if (cmpHealthHit->GetIsHurt()) {
		entity_->GetDrawCmp<CmpMesh>()->SetColor({1.0f, 0.0f, 0.0f, 1.0f});
		SEPlayer::GetInstance()->Play("./Resources/seBossHit.mp3");
		damageReactionTimer_ = damageReactionTime_;
	}
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

	for (auto& cloud : clouds_) {
		cloud->Draw();
	}

	for (auto& attack : areaAttacks_) {
		attack->Draw();
	}

	for (auto& lightning : lightningAttacks_) {

		if (lightning)
			lightning->Draw();
	}

	CommandParticle::GetInstance()->Draw("bossAppear");
	CommandParticle::GetInstance()->Draw("bossDefeat");
}

} // namespace Atrum