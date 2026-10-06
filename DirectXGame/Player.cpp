#include "Player.h"

#include "Math/Lerp.h"
#include "System/EntityFactory.h"

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

	for (auto& i : booms_) {

		i->Update();
	}
}

void Player::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	for (auto& i : booms_) {
	
		i->Draw();
	
	}

	KamataEngine::Model::PostDraw();
}

namespace Atrum {

	void CmpPlayer::ResolveDependence() {

		rigidBody_ = RefOwner().GetUpdCmp<CmpLhRigidBody>();

}
	void CmpPlayer::Initialize() {

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

			anotherOwner_->RefBooms().emplace_back(Atrum::EntityFactory::Boom(RefOwner().GetWorldPosition()));

	    }

		//
	    //--------------------------

	}

} // namespace Atrum