#include "Player.h"

using namespace KamataEngine;

void Player::Initialize(Camera* camera) {

	model_ = Model::Create();
	camera_ = camera;

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

}

void Player::Draw() {

	Model::PreDraw();

	model_->Draw(worldTransform_, *camera_);

	Model::PostDraw();

}