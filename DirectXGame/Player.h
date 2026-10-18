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

// プレイヤー
class Player final {

private:

	//----------------------------------
	//

	// 重力
	static inline const float gravity_ = 50.0f;

	// 最大となるチャージ量
	static inline const uint32_t maxCharge_ = 50;

	static inline const float chargeUIMax_ = 512.0f;

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

	// 必殺ゲージ
	uint32_t superCharge_ = 0;

	KamataEngine::Sprite* gaugeSprite_ = nullptr;

	KamataEngine::WorldTransform arrowTransform_;
	KamataEngine::Model* directionArrowModel_ = nullptr;

	// 操作方法
	KamataEngine::Sprite* inputManual_ = nullptr;

	//
	//----------------------------------

	//----------------------------------
	//

	// 最大体力
	static inline const uint32_t maxHealth_ = 11;
	uint32_t currentHealth_ = maxHealth_;

	uint32_t healthTexture_ = 0;

	KamataEngine::Sprite* healthSprites_[maxHealth_];

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

	bool onGround_ = false;

	//
	//----------------------------------

	//----------------------------------
	// 制限

	// 爆弾最大回数
	static inline const uint32_t bombMax_ = 10;

	// 爆発可能回数
	uint32_t currentBomb_ = bombMax_;

	//
	//----------------------------------

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

	void AddCharge();

	void SetArrowDireciton(const Atrum::Math::Vector2 direction);

	std::vector<std::unique_ptr<Atrum::Entity>>& RefBooms() { return booms_; }
	Atrum::Entity* GetEntity() const { return entity_.get(); }

	uint32_t GetMaxCharge() const { return maxCharge_; }
	uint32_t GetCharge() const { return superCharge_; }
	bool IsDeathFinished() const { return isDeathFinished_; }

	// 爆発可能か
	bool CanExplode();

	void ExplodeBombRemain() { currentBomb_--; }

	void RefillBombs() { currentBomb_ = bombMax_; }

	void SetCharge(uint32_t charge) { superCharge_ = charge; }

	// 着地状態取得
	bool GetOnGround() const { return onGround_; }

	// 着地状態指定
	void SetOnGround(const bool var) { onGround_ = var; }

};

namespace Atrum {

class CmpPlayer : public UpdComponent {
		
private:
	::Player* anotherOwner_ = nullptr;
	CmpLhRigidBody* rigidBody_ = nullptr;

	KamataEngine::Model* explosionFireModel_ = nullptr;
	KamataEngine::Model* explosionSmokeModel_ = nullptr;
	KamataEngine::Model* explosionRingModel_ = nullptr;

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

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::INPUT; }

	void ResolveDependence() override;
	void Initialize() override;
	void Update() override;

	void SetAnotherOwner(::Player* owner) { anotherOwner_ = owner; }

};

} // namespace Atrum
