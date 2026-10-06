#include "Player.h"

#include "Math/Lerp.h"

using namespace Atrum;
using namespace Atrum::Math;

void Player::Initialize() {

	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::PLAYER);
	entity_->SetInitialTransform({});

	entity_->AddUpdCmp<CmpLhRigidBody>();
	entity_->AddUpdCmp<CmpPlayer>();
	entity_->AddDrawCmp<CmpMesh>();

	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::Create());

	CmpLhRigidBody* cmpLhRigidBody = entity_->GetUpdCmp<CmpLhRigidBody>();
	cmpLhRigidBody->SetGravity({0.0f, -gravity_, 0.0f});
	cmpLhRigidBody->SetIsUseSpaceEnd(false);

	entity_->ResolveDependences();
	entity_->Initialize();
}

void Player::Update() {

	// cmpMesh->worldTransform_ = worldTransform_;

	entity_->Update();
}

void Player::Draw() {

	KamataEngine::Model::PreDraw();

	entity_->Draw();

	KamataEngine::Model::PostDraw();
}

namespace Atrum {

	void CmpPlayer::ResolveDependence() {

		rigidBody_ = RefOwner().GetUpdCmp<CmpLhRigidBody>();

}
	void CmpPlayer::Initialize() {

		//Vector3& velocity = rigidBody_->RefVelocity();
	    //(void)velocity;

	}

	void CmpPlayer::Update() {
	
		noGravityTime_ -= 1.0f / 60.0f;

	    if (noGravityTime_ <= 0.0f) {
		    rigidBody_->SetIsUseGravity(true);
		}

		Vector3& velocity = rigidBody_->RefVelocity();
		Vector2 direction = {};
	    Vector2 moveDirection = {};

		KamataEngine::Input *input_ = KamataEngine::Input::GetInstance();

	    if (input_->PushKey(DIK_W)) {
		    moveDirection.y += 1.0f;
	    }

	    if (input_->PushKey(DIK_S)) {
		    moveDirection.y -= 1.0f;
	    }

	    if (input_->PushKey(DIK_A)) {
		    moveDirection.x -= 1.0f;
	    }

	    if (input_->PushKey(DIK_D)) {
		    moveDirection.x += 1.0f;
	    }

	    if (input_->TriggerKey(DIK_SPACE)) {

			noGravityTime_ = noGravityTimer_;

			if (moveDirection.x == 0.0f && moveDirection.y == 0.0f) {
			    moveDirection.y = 1.0f;
			}

		    velocity.x = moveDirection.x * 30.0f;
		    velocity.y = moveDirection.y * 30.0f;

			rigidBody_->SetIsUseGravity(false);

	    }

	    // 仮地面
	   // if (transform_.translate.y <= 0.0f) {

		    //transform_.translate.y = 0.0f;
		   // velocity_.y = 0.0f;
	   // }

		//velocity.x = Atrum::Interpolation::Lerp(velocity.x, 0.0f, 0.01f);

	}

} // namespace Atrum