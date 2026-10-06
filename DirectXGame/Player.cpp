#include "Player.h"

using namespace KamataEngine;
using namespace Atrum;

void Player::Initialize() {


	entity_ = std::make_unique<Entity>();
	entity_->SetEntityType(EntityType::PLAYER);
	entity_->SetInitialTransform({});

	entity_->AddUpdCmp<CmpLhRigidBody>();
	entity_->AddDrawCmp<CmpMesh>();

	CmpMesh* cmpMesh = entity_->GetDrawCmp<CmpMesh>();
	cmpMesh->SetModel(KamataEngine::Model::Create());

	CmpLhRigidBody* cmpLhRigidBody = entity_->GetUpdCmp<CmpLhRigidBody>();
	cmpLhRigidBody->SetGravity({0.0f, -gravity_, 0.0f});

	entity_->ResolveDependences();
	entity_->Initialize();

	model_ = Model::Create();
	//camera_ = camera;

	input_ = Input::GetInstance();

}

void Player::Update() {

	 Vector2 direction = {};

	moveDirection = {};

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

	// 正規化
	float length = sqrtf(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);

	if (length != 0.0f) {
		direction.x = (moveDirection.x / length);
		direction.y = (moveDirection.y / length);
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		velocity_.x = direction.x;
		velocity_.y = direction.y;
	}

	velocity_.y -= gravity_;

	transform_.translate += velocity_;

	// 仮地面
	if (transform_.translate.y <= 0.0f) {
	
		transform_.translate.y = 0.0f;
		velocity_.y = 0.0f;

	}

	//cmpMesh->worldTransform_ = worldTransform_;

	entity_->Update();

}

void Player::Draw() {

	Model::PreDraw();

	entity_->Draw();

	Model::PostDraw();

}