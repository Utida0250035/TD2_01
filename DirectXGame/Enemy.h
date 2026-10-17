#pragma once

#include "KamataEngine.h"

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "System//CmpHitSphere.h"
#include "System//CmpMesh.h"
#include "System/CmpHealthHit.h"
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

// 取り巻き
class Enemy final {
private:
	// エンティティ
	std::unique_ptr<Atrum::Entity> entity_ = nullptr;

	Atrum::Entity* player_ = nullptr;

	// 警告マーク
	std::unique_ptr<KamataEngine::Sprite> spriteWarning_;
	bool isWarning_ = false;

	bool isDead_ = false;

public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	void EmitParticle(const Atrum::Math::Vector3& pos);

	Atrum::Entity* GetEntity() const { return entity_.get(); }
	Atrum::Entity* GetPlayer() const { return player_; }
	KamataEngine::Sprite* GetSprite() const { return spriteWarning_.get(); }
	bool GetIsDead() const { return isDead_; }

	void SetPlayer(Atrum::Entity* player) { player_ = player; }
	void SetIsWarning(bool isWarning) { isWarning_ = isWarning; }
	void SetIsDead(bool isDead) { isDead_ = isDead; }
};

namespace Atrum {

class CmpEnemy : public UpdComponent {

private:
	::Enemy* anotherOwner_ = nullptr;

	// 登場演出中か
	bool isBegin_ = true;
	// 演出時間<秒>
	static inline const float kBeginTime_ = 0.5f;
	float beginTimer_ = 0.0f;
	static inline const float kEndTime_ = 0.5f;
	float endTimer_ = 0.0f;
	Math::Vector4 entityColor_ = {1.0f, 1.0f, 1.0f, 0.0f};

	// 攻撃間隔<秒>
	static inline const float kAttackTime_ = 1.5f;
	static inline const float kBackTime_ = 0.5f;
	float attackTimer_ = 0.0f;
	bool isAttack_ = false;
	bool isBack_ = false;
	Math::Vector3 attackPos_{};
	static inline const float kAttackSpeed_ = 2.0f;

	// 警告マーク
	KamataEngine::Vector4 warningColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
	bool isToAlpha_ = true;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::MOVEMENT; }

	void ResolveDependence() override;
	void Initialize() override;
	void Update() override;

	void SetAnotherOwner(::Enemy* owner) { anotherOwner_ = owner; }
};

} // namespace Atrum