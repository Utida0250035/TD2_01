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

	std::vector<std::unique_ptr<Atrum::Entity>>& RefBooms() { return booms_; }
	Atrum::Entity* GetEntity() const { return entity_.get(); }
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
