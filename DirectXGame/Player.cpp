#include "Player.h"

#include "Math/Lerp.h"
#include "System/EntityFactory.h"
#include "Particle/CommandParticle.h"
#include "ParticleExplosionRing.h"

using namespace Atrum;
using namespace Atrum::Math;

void Player::Initialize() {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::PLAYER);
	entity_->SetInitialTransform({});

	entity_->AddUpdCmp<CmpLhRigidBody>();
	entity_->AddUpdCmp<CmpHitSphere>();
	entity_->AddUpdCmp<CmpPlayer>();
	entity_->AddDrawCmp<CmpMesh>();

	CmpPlayer* cmpPlayer = entity_->GetUpdCmp<CmpPlayer>();

	cmpPlayer->SetAnotherOwner(this);

	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::Create());

	CmpLhRigidBody* cmpLhRigidBody = entity_->GetUpdCmp<CmpLhRigidBody>();
	cmpLhRigidBody->SetGravity({0.0f, -gravity_, 0.0f});
	cmpLhRigidBody->SetIsUseSpaceEnd(false);

	CmpHitSphere* cmpHitSphere = entity_->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 2.0f}
    });

	entity_->ResolveDependences();
	entity_->Initialize();

}

void Player::Update() {

	entity_->Update();

	for (size_t i = 0; i < booms_.size(); ++i) {

		if (!booms_[i])
			continue;

		booms_[i]->Update();

		if (booms_[i]->GetState() == Entity::State::Sleep) {
		
			booms_[i]->Finalize();

			// 寿命を終えた爆発エンティティを削除
			booms_.erase(booms_.begin() + i);

			// 削除した分デクリメントする
			--i;
		
		}

	}
}

void Player::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	for (auto& i : booms_) {
	
		i->Draw();
	
	}

	CommandParticle::GetInstance()->Draw("explosion");

	KamataEngine::Model::PostDraw();
}

namespace Atrum {

	void CmpPlayer::ResolveDependence() {

		rigidBody_ = RefOwner().GetUpdCmp<CmpLhRigidBody>();

}
	void CmpPlayer::Initialize() {

		explosionFireModel_ = KamataEngine::Model::CreateFromOBJ("explosionFire", false);
		explosionSmokeModel_ = KamataEngine::Model::CreateFromOBJ("explosionSmoke", false);
		explosionRingModel_ = KamataEngine::Model::CreateFromOBJ("explosionRing", false);

	}

	void CmpPlayer::Update() {
	
		//--------------------------
		// 無重力時間

		noGravityTime_ -= 1.0f / 60.0f;

	    if (noGravityTime_ <= 0.0f) {
		    rigidBody_->SetIsUseGravity(true);
		}

		//
	    //--------------------------

		//--------------------------
		//

		// ベクトル
		Vector3& velocity = rigidBody_->RefVelocity();

		// 移動方向
	    Vector2 moveDirection = {};

		// 減衰
		velocity = Atrum::Interpolation::Lerp(velocity, {}, 0.01f);

		KamataEngine::Input *input_ = KamataEngine::Input::GetInstance();

		//
	    //--------------------------

		//--------------------------
		// 方向指定

	    if (input_->PushKey(DIK_W) || input_->PushKey(DIK_UP)) {
		    moveDirection.y += 1.0f;
	    }

	    if (input_->PushKey(DIK_S) || input_->PushKey(DIK_DOWN)) {
		    moveDirection.y -= 1.0f;
	    }

	    if (input_->PushKey(DIK_A) || input_->PushKey(DIK_LEFT)) {
		    moveDirection.x -= 1.0f;
	    }

	    if (input_->PushKey(DIK_D) || input_->PushKey(DIK_RIGHT)) {
		    moveDirection.x += 1.0f;
	    }

		//
	    //--------------------------

		//--------------------------
		// 爆発

	    if (input_->TriggerKey(DIK_SPACE)) {

		    // 無重力化
		    noGravityTime_ = noGravityTimer_;
		    rigidBody_->SetIsUseGravity(false);

		    // 方向未指定の場合は真上に指定
		    if (moveDirection.x == 0.0f && moveDirection.y == 0.0f) {
			    moveDirection.y = 1.0f;
		    }

		    // ベクトル更新
		    velocity.x = moveDirection.x * Player::jumpPower_;
		    velocity.y = moveDirection.y * Player::jumpPower_;

		    // 判定エンティティ
		    anotherOwner_->RefBooms().emplace_back(Atrum::EntityFactory::Boom(RefOwner().GetWorldPosition()));

		    ParticleExplosion* particle = new ParticleExplosion;
		    particle->SetInitialTranslation(RefOwner().GetWorldPosition());
		    particle->SetModel(explosionFireModel_, explosionSmokeModel_, explosionRingModel_);
		    particle->Initialize();

			CommandParticle::GetInstance()->Generate("explosion", particle);

	    }

		//
	    //--------------------------

	}

} // namespace Atrum