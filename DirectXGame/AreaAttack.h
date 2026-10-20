#pragma once

#include "BossAttack.h"

#include "System//CmpHitSphere.h"
#include "System//CmpMesh.h"
#include "System/CmpHealthHit.h"
#include "System/CmpLhRigidBody.h"
#include "System/Entity.h"

namespace Atrum {

class AreaAttack final : public BossAttack {

private:

	// メッシュコンポーネント
	CmpMesh* cmpMesh_ = nullptr;

	static inline const Math::Vector3 scale_ = {12, 60, 0.1f};

	// 攻撃用エンティティ
	std::unique_ptr<Entity> attackEntity_ = nullptr;

	// タイマー
	float timer_ = 0.0f;
	uint32_t intTimer_ = 0;

	// 攻撃開始時間
	static inline const float attackStartTime_ = 1.0f;

	// 警告スプライト
	KamataEngine::Sprite* warnSprite_ = nullptr;

	// 有効状態
	bool isActive_ = true;

public:

	// 初期化
	void Initialize(const Math::Vector3& emitPos) override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	bool IsActive() const { return isActive_; }

	// 終了
	void Finalize() {};
};

} // namespace Atrum
