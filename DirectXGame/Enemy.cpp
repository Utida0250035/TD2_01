#include "Enemy.h"

#include "Math/Lerp.h"
#include "System/EntityFactory.h"
#include "../KeMainCamera.h"
#include "../KeMatrix3D.h"

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
	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("unitSphere"));
	cmpMesh->SetColor({1.0f, 0.0f, 0.0f, 0.0f});
	entity_->RefTransform().scale = {0.2f, 0.2f, 0.2f};

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

	uint32_t texture = KamataEngine::TextureManager::Load("white1x1.png");
	spriteWarning_.reset(KamataEngine::Sprite::Create(texture, {0.0f, 0.0f}, {1, 1, 1, 1}, {0.5f, 0.5f}));
	spriteWarning_->SetSize({32.0f, 32.0f});
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
}

namespace Atrum {
void CmpEnemy::ResolveDependence() {}

void CmpEnemy::Initialize() {}

void CmpEnemy::Update() {
	//--------------------------
	// 登場演出

	if (isBegin_) {
		beginTimer_ += 1.0f / 60.0f;
		
		float plusScale = 0.8f * (kBeginTime_ / 60.0f);
		anotherOwner_->GetEntity()->RefTransform().scale += {plusScale, plusScale, plusScale};
		entityColor_.w += 1.0f * (kBeginTime_ / 15.0f);
		anotherOwner_->GetEntity()->GetDrawCmp<CmpMesh>()->SetColor(entityColor_);
		if (beginTimer_ >= kBeginTime_) {
			isBegin_ = false;
		}
	}

	//
	//--------------------------

	//--------------------------
	// プレイヤーに突進

	// タイマー
	if (!isAttack && !isBegin_) {
		attackTimer_ += 1.0f / 60.0f;
	}

	// 攻撃開始
	if (!isAttack && attackTimer_ >= kAttackTime_) {
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
			isAttack = true;
			attackTimer_ = 0.0f;
		}
	}

	// 攻撃
	if (isAttack) {
		// 突進
		Vector3 difference = attackPos_ - anotherOwner_->GetEntity()->GetTransform().translate;
		float t = 1.0f - std::exp(-kAttackSpeed_ * (1.0f / 60.0f));
		anotherOwner_->GetEntity()->RefTransform().translate += t * difference;
		
		// 一定距離まで近づいたら終了
		if (difference.Length() <= kAttackSpeed_) {
			isAttack = false;
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
}

} // namespace Atrum