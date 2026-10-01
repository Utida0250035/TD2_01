#include "Player.h"
#include "Easing.h"
#include "KeMatrix3D.h"
#include "KeVectorUtility.h"
#include "./Math/Lerp.h"
#include <algorithm>
#include <assert.h>
#include <numbers>

namespace {

namespace I = Atrum::Interpolation;
namespace M = Atrum::Math;

}
void Player::MoveInput(const float& deltaTime) {

	// 移動入力

	// 左右移動

	acceleration_ = {0.0f, 0.0f, 0.0f};

	if (KamataEngine::Input::GetInstance()->PushKey(DIK_RIGHT) || KamataEngine::Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 入力時

		if (KamataEngine::Input::GetInstance()->PushKey(DIK_RIGHT)) {

			if (isLanding_) {

				if (velocity_.x < 0.0f) {

					// 速度減衰
					velocity_.x *= 1.0f - kAttenuation_;
				}

				acceleration_.x += kAcceleration_;

				if (lrDirection_ != LRDirection::DIRECTION_RIGHT) {

					lrDirection_ = LRDirection::DIRECTION_RIGHT;

					// 旋回開始

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
		}

		if (KamataEngine::Input::GetInstance()->PushKey(DIK_LEFT)) {

			if (isLanding_) {

				if (velocity_.x > 0.0f) {

					// 速度減衰
					velocity_.x *= 1.0f - kAttenuation_;
				}

				acceleration_.x -= kAcceleration_;

				if (lrDirection_ != LRDirection::DIRECTION_LEFT) {

					lrDirection_ = LRDirection::DIRECTION_LEFT;

					// 旋回開始

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
		}

	} else {
		// 非入力時

		if (isLanding_) {

			// 速度減衰
			velocity_.x *= 1.0f - kAttenuation_;
		}
	}

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_UP)) {

		if (isLanding_) {

			Jump();
		}
	}

	acceleration_.y += kGravityAcceleration;

	// 加減速
	velocity_ += acceleration_ * deltaTime;

	// 速度上下限
	velocity_.x = std::clamp(velocity_.x, -kHorizontalSpeedLimit, kHorizontalSpeedLimit);

	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);
}

void Player::TopCollisionMap(CollisionMapInfo& info) {

	if (info.displacement.y <= 0.0f) {
		// Y軸において下降あるいは静止

		return;
	}

	std::array<M::Vector3, 4> positionsNew{};

	// ワールド頂点の取得

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = GetCornerPosition(FromKamataEngine(worldTransform_.translation_) + info.displacement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType nextMapChipType;

	bool isHit = false;

	MapIndexSet indexSet;

	// 左上の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_LEFT_TOP]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex + 1);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	// 右上の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_RIGHT_TOP]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex + 1);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	if (isHit) {
		// 天井判定と押し戻し

		indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_LEFT_TOP]);

		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		if (worldTransform_.translation_.y + kHitHeight_ / 2.0f - kCollisionBlank_ <= rect.bottom) {

			info.displacement.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - kHitHeight_ / 2.0f - kCollisionBlank_);

			info.isCollisionCeil = true;
		}
	}
}

void Player::BottomCollisionMap(CollisionMapInfo& info) {
	if (info.displacement.y >= 0.0f) {
		// Y軸において上昇あるいは静止

		return;
	}

	// ワールド頂点の取得
	std::array<M::Vector3, 4> positionsNew{};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = GetCornerPosition(FromKamataEngine(worldTransform_.translation_) + info.displacement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType nextMapChipType;

	bool isHit = false;

	MapIndexSet indexSet;

	// 左下の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_LEFT_BOTTOM]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	// 右下の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_RIGHT_BOTTOM]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex - 1);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	if (isHit) {
		// 着地判定と押し戻し

		indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_LEFT_BOTTOM]);

		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		if (worldTransform_.translation_.y - kHitHeight_ / 2.0f + kCollisionBlank_ >= rect.top) {

			info.displacement.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + kHitHeight_ / 2.0f + kCollisionBlank_);

			info.isLanding = true;
		}
	}
}

void Player::RightCollisionMap(CollisionMapInfo& info) {
	if (velocity_.x <= 0.0f) {
		// X軸において左移動または静止

		return;
	}

	// ワールド頂点の取得
	std::array<M::Vector3, 4> positionsNew{};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = GetCornerPosition(FromKamataEngine(worldTransform_.translation_) + info.displacement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType nextMapChipType;

	bool isHit = false;

	MapIndexSet indexSet;

	// 右上の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_RIGHT_TOP]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex - 1, indexSet.yIndex);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	// 右下の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_RIGHT_BOTTOM]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex - 1, indexSet.yIndex);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	if (isHit) {
		// 右衝突判定と押し戻し

		indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_RIGHT_BOTTOM]);

		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		if (worldTransform_.translation_.x + kHitWidth_ / 2.0f - kCollisionBlank_ <= rect.left) {

			info.displacement.x = std::min(0.0f, rect.left - worldTransform_.translation_.x - kHitWidth_ / 2.0f - kCollisionBlank_);

			info.isCollisionWallRight = true;
		}
	}
}

void Player::LeftCollisionMap(CollisionMapInfo& info) {
	if (velocity_.x >= 0.0f) {
		// X軸において右移動または静止

		return;
	}

	// ワールド頂点の取得
	std::array<M::Vector3, 4> positionsNew{};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {

		positionsNew[i] = GetCornerPosition(FromKamataEngine(worldTransform_.translation_) + info.displacement, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType nextMapChipType;

	bool isHit = false;

	MapIndexSet indexSet;

	// 左上の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_LEFT_TOP]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex + 1, indexSet.yIndex);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	// 左下の判定

	indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_LEFT_BOTTOM]);
	mapChipType = mapChipField_->GetMapCellType(indexSet.xIndex, indexSet.yIndex);
	nextMapChipType = mapChipField_->GetMapCellType(indexSet.xIndex + 1, indexSet.yIndex);

	if (mapChipType == MapChipType::CHIP_BLOCK && nextMapChipType != MapChipType::CHIP_BLOCK) {

		isHit = true;
	}

	if (isHit) {
		// 左衝突判定と押し戻し

		indexSet = mapChipField_->GetMapCellIndex(positionsNew[CORNER_LEFT_BOTTOM]);

		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		if (worldTransform_.translation_.x - kHitWidth_ / 2.0f + kCollisionBlank_ >= rect.right) {

			info.displacement.x = std::max(0.0f, rect.right - worldTransform_.translation_.x + kHitWidth_ / 2.0f + kCollisionBlank_);

			info.isCollisionWallLeft = true;
		}
	}
}

void Player::CollisionMap(CollisionMapInfo& info) {
	TopCollisionMap(info);
	BottomCollisionMap(info);
	CheckLanding(info);
	RightCollisionMap(info);
	LeftCollisionMap(info);
	CheckCollisionWall(info);
}

void Player::ApplyDisplacement(CollisionMapInfo& info) { worldTransform_.translation_ += ToKamataEngine(info.displacement); }

void Player::CollisionCeil(CollisionMapInfo& info) {

	if (info.isCollisionCeil) {

		velocity_.y = 0.0f;
	}
}

void Player::CheckLanding(const CollisionMapInfo& info) {

	if (info.isLanding) {

		isLanding_ = true;
		velocity_.y = 0.0f;

		attackEnergy_ = attackEnergyMax_;
	}
}

void Player::CheckCollisionWall(const CollisionMapInfo& info) {

	if (info.isCollisionWallLeft || info.isCollisionWallRight) {

		velocity_.x *= (1.0f - kAttenuationCollisionWall_);
	}
}

void Player::ClampToCameraRange(const CollisionMapInfo& info) {

	if (worldTransform_.translation_.x - camera_->translation_.x >= 7.0f) {

		worldTransform_.translation_.x = camera_->translation_.x + 7.0f;

	} else if (worldTransform_.translation_.x - camera_->translation_.x <= -7.0f) {

		worldTransform_.translation_.x = camera_->translation_.x - 7.0f;

		if (info.isCollisionWallRight) {

			isAlive_ = false;

			velocity_ = {0.0f, 0.3f, 0.0f};
		}
	}
}

void Player::Initialize(KamataEngine::Model* model, const uint32_t& textureHandle, KamataEngine::Model* attackModel, KamataEngine::Camera* camera, const M::Vector3& position) {

	assert(model && "This 3D model is nullptr");

	model_ = model;
	textureHandle_ = textureHandle;

	attackModel_ = attackModel;

	worldTransform_.Initialize();
	worldTransform_.translation_ = ToKamataEngine(position);

	attackWorldTransform_.Initialize();

	float destinationRotationYTable[] = {std::numbers::pi_v<float> * 1.5f, std::numbers::pi_v<float> / 2.0f};

	worldTransform_.rotation_.y = destinationRotationYTable[static_cast<size_t>(lrDirection_)];

	camera_ = camera;

	velocity_ = {0.0f, 0.0f, 0.0f};
}

void Player::BehaviorRootInitialize() {}

void Player::BehaviorRootUpdate(const float deltaTime) {

	MoveInput(deltaTime);

	if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {

		if (attackEnergy_ >= 1) {

			attackEnergy_--;

			behaviorRequest_ = Behavior::kAttack;
		}
	}
}

void Player::BehaviorAttackInitialize() {
	attackTimeParameter_ = 0.0f;
	attackPhase_ = AttackPhase::kFocus;

	velocity_ = {};
}

void Player::BehaviorAttackUpdate(const float deltaTime) {

	attackTimeParameter_ += deltaTime;

	float t = 0.0f;

	switch (attackPhase_) {

	case AttackPhase::kFocus:
	default:

		t = attackTimeParameter_ / focusTimeLimit_;
		worldTransform_.scale_.z = I::Lerp(1.0f, 0.3f, I::EaseOutCirc(t));
		worldTransform_.scale_.y = I::Lerp(1.0f, 1.6f, I::EaseOutQuad(t));

		if (attackTimeParameter_ >= focusTimeLimit_) {

			attackPhase_ = AttackPhase::kRush;
			attackTimeParameter_ = 0.0f;

			if (lrDirection_ == LRDirection::DIRECTION_LEFT) {

				velocity_ = {-rushAttackSpeed_, 0.0f, 0.0f};

			} else {

				velocity_ = {rushAttackSpeed_, 0.0f, 0.0f};
			}
		}

		break;

	case AttackPhase::kRush:

		t = attackTimeParameter_ / rushTimeLimit_;

		worldTransform_.scale_.z = I::Lerp(0.3f, 1.3f, I::EaseOutQuart(t));
		worldTransform_.scale_.y = I::Lerp(1.6f, 0.7f, I::EaseInCirc(t));

		if (attackTimeParameter_ >= rushTimeLimit_) {

			attackTimeParameter_ = 0.0f;

			attackPhase_ = AttackPhase::kAfter;
		}

		break;

	case AttackPhase::kAfter:

		t = attackTimeParameter_ / attackAfterTimeLimit_;

		worldTransform_.scale_.z = I::Lerp(1.3f, 1.0f, I::EaseOutCirc(t));

		worldTransform_.scale_.y = I::Lerp(0.7f, 1.0f, I::EaseOutQuad(t));

		if (attackTimeParameter_ >= attackAfterTimeLimit_) {

			behaviorRequest_ = Behavior::kRoot;
			attackPhase_ = AttackPhase::kFocus;
		}

		break;
	}
}

void Player::Update(const float deltaTime) {

	if (!isAlive_) {

		ImGui();

		return;
	}

	if (behaviorRequest_ != Behavior::kUnknown) {

		behavior_ = behaviorRequest_;

		switch (behavior_) {

		case Behavior::kRoot:
		default:

			BehaviorRootInitialize();

			break;

		case Behavior::kAttack:

			BehaviorAttackInitialize();

			break;
		}

		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {

	case Behavior::kRoot:

		BehaviorRootUpdate(deltaTime);

		break;

	case Behavior::kAttack:

		BehaviorAttackUpdate(deltaTime);

		break;
	}

	if (turnTimer_ > 0.0f) {

		turnTimer_ -= deltaTime;

		// 旋回制御

		float destinationRotationYTable[] = {std::numbers::pi_v<float> * 1.5f, std::numbers::pi_v<float> / 2.0f};

		worldTransform_.rotation_.y = I::Lerp(turnFirstRotationY_, destinationRotationYTable[static_cast<size_t>(lrDirection_)], I::EaseOutQuad((kTimeTurn - turnTimer_) / kTimeTurn));
	}

	CollisionMapInfo collisionMapInfo{};

	collisionMapInfo.displacement = velocity_ * deltaTime;

	CollisionMap(collisionMapInfo);

	ApplyDisplacement(collisionMapInfo);

	CollisionCeil(collisionMapInfo);

	ClampToCameraRange(collisionMapInfo);

	if (worldTransform_.translation_.y <= -5.0f) {

		isAlive_ = false;
	}

	attackWorldTransform_.translation_ = worldTransform_.translation_;
	attackWorldTransform_.rotation_ = worldTransform_.rotation_;

	// ワールド変換
	UpdateWorldTransform(worldTransform_);
	UpdateWorldTransform(attackWorldTransform_);

	ImGui();
}

void Player::Draw() {

	if (!isAlive_) {
		return;
	}

	model_->Draw(worldTransform_, *camera_, textureHandle_);

	if (behavior_ == Behavior::kAttack) {

		if (attackPhase_ != AttackPhase::kFocus) {

			attackModel_->Draw(attackWorldTransform_, *camera_);
		}
	}
}

void Player::Jump() {

	velocity_.y = kJumpVelocityOrigin_;
	isLanding_ = false;
}

void Player::ImGui() {

#ifdef _DEBUG

	ImGui::Begin("playerInfo");

	ImGui::Text("playerPos: %.0f(x), %.0f(y)", worldTransform_.translation_.x, worldTransform_.translation_.y);

	ImGui::End();

#endif
}

M::Vector3 Player::GetPosWorld() const { return FromKamataEngine(KamataEngine::Vector3{worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]}); }

AABB Player::GetAABB() const {

	M::Vector3 posWorld = GetPosWorld();

	return AABB{
	    M::Vector3{posWorld.x - kHitWidth_ * 0.5f, posWorld.y - kHitHeight_ * 0.5f, posWorld.z - 1.0f},
        M::Vector3{posWorld.x + kHitWidth_ * 0.5f, posWorld.y + kHitHeight_ * 0.5f, posWorld.z + 1.0f}
    };
}