#include "Enemy.h"

#include "Math/Lerp.h"
#include "System/EntityFactory.h"
#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"
#include "Particle/CommandParticle.h"
#include "../SEPlayer.h"

#include "ParticleEnemyAppear.h"

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
	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("enemy"));

	CmpHitSphere* cmpHitSphere = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 1.0f}
    });
	cmpHitSphere->SetIsActive(false);

	cmpHitSphere->SetEnemy();

	CmpHealthHit* cmpHealthHit = entity_->GetUpdCmp<CmpHealthHit>();
	std::vector<EntityType> hurterTypes;
	hurterTypes.push_back(EntityType::PLAYER_WEAPON);
	cmpHealthHit->SetHurterTypes(hurterTypes);

	entity_->ResolveDependences();
	entity_->Initialize();

	cmpMesh->SetColor({0.0f, 0.0f, 0.0f, 0.0f});
	entity_->RefTransform().scale = {0.2f, 0.2f, 0.2f};

	uint32_t texture = KamataEngine::TextureManager::Load("warning.png");
	spriteWarning_.reset(KamataEngine::Sprite::Create(texture, {0.0f, 0.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));

	SEPlayer::GetInstance()->Play("./Resources/seEnemyAppear.mp3");
}

void Enemy::Update() { entity_->Update(); }

void Enemy::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	KamataEngine::Model::PostDraw();

	KamataEngine::Sprite::PreDraw();

	if (isWarning_) {
		spriteWarning_->Draw();
	}

	KamataEngine::Sprite::PostDraw();

	CommandParticle::GetInstance()->Draw("enemyAppear");
}

void Enemy::EmitParticle(const Vector3& pos) {
	ParticleEnemyAppear* particleAppear = new ParticleEnemyAppear;
	particleAppear->SetModel(KamataEngine::Model::CreateFromOBJ("cloudParticle", false));
	particleAppear->SetTargetPosition(pos);
	particleAppear->Initialize();
	CommandParticle::GetInstance()->Generate("enemyAppear", particleAppear);
}

namespace Atrum {
void CmpEnemy::ResolveDependence() {}

void CmpEnemy::Initialize() {}

void CmpEnemy::Update() {

	//--------------------------
	// 登場演出
	if (isBegin_) {
		beginTimer_ += 1.0f / 60.0f;
		float plusScale = 0.8f / (kBeginTime_ * 60.0f);
		anotherOwner_->GetEntity()->RefTransform().scale += {plusScale, plusScale, plusScale};
		entityColor_.w += 1.0f / (kBeginTime_ * 60.0f);
		anotherOwner_->GetEntity()->GetDrawCmp<CmpMesh>()->SetColor(entityColor_);
		if (beginTimer_ >= kBeginTime_) {
			isBegin_ = false;
			anotherOwner_->GetEntity()->GetUpdCmp<CmpHitSphere>()->SetIsActive(true);
		}
	}

	//
	//--------------------------

	//--------------------------
	// プレイヤーに突進

	// タイマー
	if (!isAttack_ && !isBegin_) {
		attackTimer_ += 1.0f / 60.0f;
	}

	// 予備動作開始
	if (!isAttack_ && attackTimer_ >= kAttackTime_) {
		isBack_ = true;
		anotherOwner_->SetIsWarning(true);
		attackTimer_ = 0.0f;
		attackPos_ = anotherOwner_->GetPlayer()->GetTransform().translate;
	}

	// 予備動作
	if (isBack_) {
		// 後ろに下がる
		Vector3 difference = attackPos_ - anotherOwner_->GetEntity()->GetTransform().translate;
		float t = 1.0f - std::exp((kAttackSpeed_ / 5.0f) * (1.0f / 60.0f));
		anotherOwner_->GetEntity()->RefTransform().translate += t * difference;

		if (attackTimer_ >= kBackTime_) {
			isBack_ = false;
			anotherOwner_->SetIsWarning(false);
			isAttack_ = true;
			attackTimer_ = 0.0f;
			anotherOwner_->GetEntity()->SetEntityType(EntityType::ENEMY_WEAPON);
			SEPlayer::GetInstance()->Play("./Resources/seEnemyAttack.mp3");
			anotherOwner_->GetEntity()->GetDrawCmp<CmpMesh>()->SetColor({1.0f, 1.0f, 0.1f, 1.0f});
		}
	}

	// 攻撃
	if (isAttack_) {
		// 突進
		Vector3 difference = attackPos_ - anotherOwner_->GetEntity()->GetTransform().translate;
		float t = 1.0f - std::exp(-kAttackSpeed_ * (1.0f / 60.0f));
		anotherOwner_->GetEntity()->RefTransform().translate += t * difference;
		
		// 一定距離まで近づいたら終了
		if (difference.Length() <= kAttackSpeed_ * 1.0f) {
			if (difference.Length() <= kAttackSpeed_) {
				isAttack_ = false;
			}
			anotherOwner_->GetEntity()->SetEntityType(EntityType::ENEMY);
			anotherOwner_->GetEntity()->GetDrawCmp<CmpMesh>()->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
		}
	}

	//
	//--------------------------

	//--------------------------
	// 警告マーク

	// ビューポート
	Matrix4x4 matViewport = Matrix4x4::Viewport(0.0f, 0.0f, float(KamataEngine::WinApp::kWindowWidth), float(KamataEngine::WinApp::kWindowHeight), 0.0f, 1.0f);

	// 合成
	Matrix4x4 matView = FromKamataEngine(KeMainCamera::GetInstance()->camera_->matView);
	Matrix4x4 matProjection = FromKamataEngine(KeMainCamera::GetInstance()->camera_->matProjection);
	Matrix4x4 matViewProjection = matView * matProjection;
	
	// 座標変換
	Vector3 screenPos = Matrix4x4::ScreenTransform(anotherOwner_->GetEntity()->GetTransform().translate, matViewProjection, matViewport);
	anotherOwner_->GetSprite()->SetPosition({screenPos.x, screenPos.y});


	// 透明度
	if (isToAlpha_) {
		warningColor_.w -= 1.0f / 15.0f;
		if (warningColor_.w <= 0.3f) {
			warningColor_.w = 0.3f;
			isToAlpha_ = false;
		}
	} else {
		warningColor_.w += 1.0f / 15.0f;
		if (warningColor_.w >= 1.0f) {
			warningColor_.w = 1.0f;
			isToAlpha_ = true;
		}
	}

	anotherOwner_->GetSprite()->SetColor(warningColor_);

	//
	//--------------------------

	//--------------------------
	// 死亡演出

	if (anotherOwner_->GetEntity()->GetUpdCmp<CmpHealthHit>()->GetHealthCount() <= 0) {
		if (endTimer_ == 0.0f) {
			anotherOwner_->GetEntity()->GetUpdCmp<CmpHitSphere>()->SetIsActive(false);
			anotherOwner_->EmitParticle(anotherOwner_->GetEntity()->GetWorldPosition());
			SEPlayer::GetInstance()->Play("./Resources/seEnemyHit.mp3");
		}
		endTimer_ += 1.0f / 60.0f;

		anotherOwner_->SetIsWarning(false);
		isAttack_ = false;
		isBack_ = false;

		float minusScale = 0.8f / (kEndTime_ * 60.0f);
		anotherOwner_->GetEntity()->RefTransform().scale -= {minusScale, minusScale, minusScale};
		entityColor_.w -= 1.0f / (kEndTime_ * 60.0f);
		anotherOwner_->GetEntity()->GetDrawCmp<CmpMesh>()->SetColor(entityColor_);
		if (endTimer_ >= kEndTime_) {
			anotherOwner_->SetIsDead(true);
		}
	}

	//
	//--------------------------
}

} // namespace Atrum