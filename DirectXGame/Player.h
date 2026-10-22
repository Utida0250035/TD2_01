#pragma once

#include "KamataEngine.h"

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "System//CmpMesh.h"
#include "System//CmpHitSphere.h"
#include "System/CmpLhRigidBody.h"
#include "System/Entity.h"

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "../Hash/Hash64.h"
#include "../Math/Transform.h"
#include "../System/DataComponent.h"
#include "../System/DrawComponent.h"
#include "../System/EntityType.h"
#include "../System/UpdComponent.h"
#include "../System/CmpHealthHit.h"

#include <iostream>

class Floor;

// プレイヤー
class Player final {
public:
	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool isHitCeiling = false;        // 天井
		bool island = false;              // 着地
		bool isHitFloor = false;          // 床
		bool isHitWall = false;           // 壁
		Atrum::Math::Vector3 moveAmount; // 移動量
	};

	// 方向
	enum class Direction {
		kTop,
		kBottom,
		kRight,
		kLeft,

		kNumDirection // 要素数
	};

	// 角
	enum class Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner // 要素数
	};

private:

	//----------------------------------
	//

	// 重力
	static inline const float gravity_ = 50.0f;

	float deathVelocity_ = 0.0f;

	//
	//----------------------------------

	// 入力
	KamataEngine::Input* input_ = nullptr;

	// エンティティ
	std::unique_ptr<Atrum::Entity> entity_ = nullptr;

	// 爆発エンティティ
	std::vector<std::unique_ptr<Atrum::Entity>> booms_{};

	//----------------------------------
	//

	KamataEngine::WorldTransform arrowTransform_;
	KamataEngine::Model* directionArrowModel_ = nullptr;

	uint32_t keyboardManual_ = 0;
	uint32_t xboxControllerManual_ = 0;

	// 操作方法
	KamataEngine::Sprite* inputManual_ = nullptr;
	
	//残弾数
	std::vector<KamataEngine::Sprite*> numbers_;
	KamataEngine::Sprite* bombSprite_ = nullptr;
	uint32_t bombCount_;
	uint32_t bombNum_[2];
	Atrum::Math::Vector3 bombNumPos_;
	static inline const Atrum::Math::Vector3 bombNumOffset_ = {-2.0f, -1.0f, 0.0f};

	//
	//----------------------------------

	//----------------------------------
	//

	// 最大体力
	static inline const uint32_t maxHealth_ = 5;
	uint32_t currentHealth_ = maxHealth_;

	uint32_t healthTexture_ = 0;

	KamataEngine::Sprite* healthSprites_[maxHealth_];
	KamataEngine::Sprite* healthNoneSprites_[maxHealth_];

	Atrum::CmpHealthHit* cmpHealthHit_ = nullptr;

	// 無敵タイマー
	float invincibleTimer_ = 0.0f;

	// 被弾時の無敵時間
	static inline const float invincibleTime_ = 1.0f;

	// 無敵状態
	bool isInvincible_ = false;

	// 死亡状態
	bool isDeath_ = false;

	// 死亡時に加算するタイマー
	float deathTimer_ = 0.0f;

	// 死亡演出にかける時間
	float deathTime_ = 2.0f;

	// 死亡演出終了状態
	bool isDeathFinished_ = false;

	bool isDeathBoss_ = false;

	bool onGround_ = false;

	// 被弾時の閃光スプライト
	KamataEngine::Sprite* damagedFlash_ = nullptr;

	// 閃光アルファ値
	float flashAlpha_ = 0.0f;

	//
	//----------------------------------

	//----------------------------------
	// 残弾

	// 残弾最大数
	static inline const uint32_t bombMax_ = 10;

	// 残弾数
	uint32_t currentBomb_ = bombMax_;

	//
	//----------------------------------

	Floor* floor_ = nullptr;

public:
	static inline const float jumpPower_ = 50.0f;

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// UI描画
	void DrawUI();

	void SetArrowDireciton(const Atrum::Math::Vector2 direction);

	std::vector<std::unique_ptr<Atrum::Entity>>& RefBooms() { return booms_; }
	Atrum::Entity* GetEntity() const { return entity_.get(); }

	bool IsDeathFinished() const { return isDeathFinished_; }

	// 爆発可能か
	bool CanExplode();

	void ExplodeBombRemain() { currentBomb_--; }

	void RefillBombs() { currentBomb_ = bombMax_; }


	// 着地状態取得
	bool GetOnGround() const { return onGround_; }

	// 着地状態指定
	void SetOnGround(const bool var) { onGround_ = var; }

	void SetFloor(Floor* floor) { floor_ = floor; }
	Floor* GetFloor() { return floor_; }

	void SetIsDeathBoss(bool isDeathBoss) { isDeathBoss_ = isDeathBoss; }

	Atrum::Math::Vector3* GetTranslatePointer() { return &entity_->RefTransform().translate; }

};

namespace Atrum {

class CmpPlayer : public UpdComponent {
		
private:
	::Player* anotherOwner_ = nullptr;
	CmpLhRigidBody* rigidBody_ = nullptr;

	KamataEngine::Model* explosionFireModel_ = nullptr;
	KamataEngine::Model* explosionSmokeModel_ = nullptr;
	KamataEngine::Model* explosionRingModel_ = nullptr;
	KamataEngine::Model* collisionRingModel_ = nullptr;

	// 無重力時間
	static inline const float noGravityTimer_ = 0.2f;

	// X軸移動制限
	static inline const float xVectorMoveLimit_ = 40.0f;

	// 上方向移動制限
	static inline const float upperMoveLimit_ = 22.0f;

	// 残り無重力時間
	float noGravityTime_ = 0.0f;

	// テスト用に用意した床判定の座標
	static inline const float testGroundPos = -20.0f;

	// 当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
	static inline const float kBlank = 0.1f;

	// 壁接触時の減衰率
	static inline const float kAttenuationWall = 0.3f;

	// 最大速度
	static inline const float kLimitSpeed_ = 1.0f;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::INPUT; }

	void ResolveDependence() override;
	void Initialize() override;
	void Update() override;

	void CollisionMap(::Player::CollisionMapInfo& info);
	void CollisionMapAllDirections(::Player::CollisionMapInfo& info, ::Player::Direction& direction);

	void Move(::Player::CollisionMapInfo& info);

	void HitCeilingUpdate(const ::Player::CollisionMapInfo& info);
	void HitWallUpdate(const ::Player::CollisionMapInfo& info);
	void HitFloorUpdate(const ::Player::CollisionMapInfo& info);

	Math::Vector3 CornerPosition(const Math::Vector3& center, ::Player::Corner corner);

	void SetAnotherOwner(::Player* owner) { anotherOwner_ = owner; }

};

} // namespace Atrum
