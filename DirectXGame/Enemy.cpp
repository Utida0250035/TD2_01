#include "Enemy.h"
#include "KeMatrix3D.h"
#include "KeVectorUtility.h"
#include "./Math/Lerp.h"
#include "Easing.h"
#include <algorithm>
#include <assert.h>
#include <numbers>

namespace {

namespace I = Atrum::Interpolation;
namespace M = Atrum::Math;

}

void Enemy::Move(const float deltaTime) {

	// 移動

	// 左右移動

	// 往復時間
	roundTripTimer_ += deltaTime;

	// 移動方向
	if (roundTripTimer_ >= kRoundTripTime_ * 0.5f) {

		if (roundTripTimer_ >= kRoundTripTime_) {

			roundTripTimer_ = 0.0f;
		}

		// 左へ

		acceleration_ = {-kAcceleration_, 0.0f, 0.0f};

	} else {

		// 右へ

		acceleration_ = {kAcceleration_, 0.0f, 0.0f};
	}

	// 加速

	velocity_ += acceleration_;

	velocity_.y += kGravityAcceleration * deltaTime;

	// 速度上下限
	velocity_.x = std::clamp(velocity_.x, -kHorizontalSpeedLimit, kHorizontalSpeedLimit);

	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);

	if (velocity_.x < 0.0f) {

		if (lrDirection_ != LRDirection::DIRECTION_LEFT) {

			lrDirection_ = LRDirection::DIRECTION_LEFT;

			turnTimer_ = kTurnTime_;

			turnFirstRotationY_ = worldTransform_.rotation_.y;
		}

	} else if (velocity_.x > 0.0f) {

		if (lrDirection_ != LRDirection::DIRECTION_RIGHT) {

			lrDirection_ = LRDirection::DIRECTION_RIGHT;

			turnTimer_ = kTurnTime_;

			turnFirstRotationY_ = worldTransform_.rotation_.y;
		}
	}
}

void Enemy::MoveMotion(const float deltaTime) {

	walkMotionTimer_ += deltaTime;

	if (walkMotionTimer_ >= kWalkMotionTime_) {

		walkMotionTimer_ = 0.0f;
	}

	float param = I::EaseInQuad(std::sin(std::numbers::pi_v<float> * 2.0f * walkMotionTimer_ / kWalkMotionTime_));

	worldTransform_.rotation_.x = kWalkMotionAngleStart_ + kWalkMotionAngleEnd_ * (param + 1.0f) * 0.5f;

}

void Enemy::MoveTurn(const float deltaTime) {

	if (turnTimer_ > 0.0f) {

		turnTimer_ -= deltaTime;

		// 旋回制御

		float destinationRotationYTable[] = {std::numbers::pi_v<float> * 1.5f, std::numbers::pi_v<float> / 2.0f};

		worldTransform_.rotation_.y = I::Lerp(I::EaseOutQuad((kTurnTime_ - turnTimer_) / kTurnTime_), turnFirstRotationY_, destinationRotationYTable[static_cast<size_t>(lrDirection_)]);
	
	}

}

void Enemy::DeathMotion(const float deltaTime) {

	deathTimer_ += deltaTime;
	
	worldTransform_.rotation_.y += deathRotateSpeed_ * deltaTime;
	worldTransform_.scale_.x -= deathShrinkSpeed_ * deltaTime;
	worldTransform_.scale_.x = std::max(0.0f, worldTransform_.scale_.x);
	worldTransform_.scale_.z -= deathShrinkSpeed_ * deltaTime;
	worldTransform_.scale_.z = std::max(0.0f, worldTransform_.scale_.z);

	if (deathTimer_ >= timeForDeath_) {

		isAlive_ = false;
	}
	

}

void Enemy::TopCollisionMap(CollisionMapInfo& info) {

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
			// プレイヤー―の上辺がセルの下辺以下であれば(セル境界チェック)

			info.displacement.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - kHitHeight_ / 2.0f - kCollisionBlank_);

			info.isCollisionCeil = true;
		}
	}
}

void Enemy::BottomCollisionMap(CollisionMapInfo& info) {
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
			// プレイヤー―の下辺がセルの上辺以上であれば(セル境界チェック)

			info.displacement.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + kHitHeight_ / 2.0f + kCollisionBlank_);

			info.isLanding = true;
		}
	}
}

void Enemy::RightCollisionMap(CollisionMapInfo& info) {
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
			// プレイヤー―の右辺がセルの左辺以下であれば(セル境界チェック)

			info.displacement.x = std::min(0.0f, rect.left - worldTransform_.translation_.x - kHitWidth_ / 2.0f - kCollisionBlank_);

			info.isCollisionWall = true;
		}
	}
}

void Enemy::LeftCollisionMap(CollisionMapInfo& info) {
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
			// プレイヤー―の左辺がセルの右辺以上であれば(セル境界チェック)

			info.displacement.x = std::max(0.0f, rect.right - worldTransform_.translation_.x + kHitWidth_ / 2.0f + kCollisionBlank_);

			info.isCollisionWall = true;
		}
	}
}

void Enemy::CollisionMap(CollisionMapInfo& info) {
	TopCollisionMap(info);
	BottomCollisionMap(info);
	CheckLanding(info);
	RightCollisionMap(info);
	LeftCollisionMap(info);
	CheckCollisionWall(info);
}

void Enemy::ApplyDisplacement(CollisionMapInfo& info) { worldTransform_.translation_ += ToKamataEngine(info.displacement); }

void Enemy::CollisionCeil(CollisionMapInfo& info) {

	if (info.isCollisionCeil) {

		velocity_.y = 0.0f;
	}
}

void Enemy::CheckLanding(const CollisionMapInfo& info) {

	if (info.isLanding) {

		isLanding_ = true;
		velocity_.y = 0.0f;

	}
}

void Enemy::CheckCollisionWall(const CollisionMapInfo& info) {

	if (info.isCollisionWall) {

		velocity_.x *= (1.0f - kAttenuationCollisionWall_);
	}
}

void Enemy::Initialize(KamataEngine::Model* model, const uint32_t& textureHandle, KamataEngine::Camera* camera, const M::Vector3& position) {

	assert(model && "This 3D model is nullptr");

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = ToKamataEngine(position);

	float destinationRotationYTable[] = {std::numbers::pi_v<float> * 1.5f, std::numbers::pi_v<float> / 2.0f};

	worldTransform_.rotation_.y = destinationRotationYTable[static_cast<size_t>(lrDirection_)];

	camera_ = camera;
}

void Enemy::Update(const float deltaTime) {

	if (!isCollisionAble_) {
	
		DeathMotion(deltaTime);
	
	}

	MoveMotion(deltaTime);

	Move(deltaTime);

	CollisionMapInfo collisionMapInfo{};

	collisionMapInfo.displacement = velocity_ * deltaTime;

	CollisionMap(collisionMapInfo);

	ApplyDisplacement(collisionMapInfo);

	CollisionCeil(collisionMapInfo);

	MoveTurn(deltaTime);

	// ワールド変換
	UpdateWorldTransform(worldTransform_);

	ImGui();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_, textureHandle_); }

void Enemy::ImGui() {

#ifdef _DEBUG

	ImGui::Begin("enemyInfo");

	ImGui::Text("enemyPos: %.0f(x), %.0f(y)", worldTransform_.translation_.x, worldTransform_.translation_.y);

	ImGui::End();

#endif
}

AABB Enemy::GetAABB() const {

	M::Vector3 posWorld = GetPosWorld();

	return AABB{
	    M::Vector3{posWorld.x - kHitWidth_ * 0.5f, posWorld.y - kHitHeight_ * 0.5f, posWorld.z - 1.0f},
        M::Vector3{posWorld.x + kHitWidth_ * 0.5f, posWorld.y + kHitHeight_ * 0.5f, posWorld.z + 1.0f}
    };
}