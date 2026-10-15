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

#include "BossLightning.h"
#include "Enemy.h"

namespace Atrum {

class FrameDeltaTime;

// ボス
class Boss final {
private:

	// 時間計測
	FrameDeltaTime* frameDeltaTime_ = nullptr;

	// エンティティ
	std::unique_ptr<Atrum::Entity> entity_ = nullptr;

	// 取り巻き
	std::vector<std::unique_ptr<Enemy>> enemies_{};

	// 雷攻撃
	std::vector<std::unique_ptr<Atrum::BossLightning>> lightningAttacks_{};

	// 攻撃ループ用タイマー
	float attackLoopTimer_ = 0.0f;

	// 1攻撃ループの時間
	float attackLoopTime_ = 3.0f;

	// 召喚ループ用タイマー
	float summonLoopTimer_ = 0.0f;

	// 1召喚ループの時間
	float summonLoopTime_ = 0.5f;

	Math::Vector3 lightningAttackOffsets_[2] = {
	    {2.5f,  2.5f, 0.0f},
        {-2.5f, 2.5f, 0.0f}
    };

public:
	~Boss();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	Atrum::Entity* GetEntity() const { return entity_.get(); }
	std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }
};

} // namespace Atrum
