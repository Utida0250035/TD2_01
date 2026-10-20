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
#include "BossCloud.h"
#include "AreaAttack.h"

#include "../Camera/CameraController.h"

namespace Atrum {

class FrameDeltaTime;

// ボス
class Boss final {
private:

	// 行動列挙型
	enum Actions {

		Move,

		EnemySummon,

		CloudSummon,

		AreaSummon,

		Count,

	};

	// 時間計測
	FrameDeltaTime* frameDeltaTime_ = nullptr;

	// エンティティ
	std::unique_ptr<Atrum::Entity> entity_ = nullptr;

	// 取り巻き
	std::vector<std::unique_ptr<Enemy>> enemies_{};

	// 曇
	std::vector<std::unique_ptr<BossCloud>> clouds_{};

	// 範囲攻撃
	std::vector<std::unique_ptr<AreaAttack>> areaAttacks_{};

	// 雷攻撃
	std::vector<std::unique_ptr<Atrum::BossLightning>> lightningAttacks_{};

	// 攻撃ループ用タイマー
	float attackLoopTimer_ = 0.0f;

	// 1攻撃ループの時間
	float attackLoopTime_ = 3.0f;

	// 雲召喚タイマー
	float cloudLoopTimer_ = 0.0f;

	// 雲召喚間隔
	float cloudLoopTime_ = 3.0f;

	// 召喚ループ用タイマー
	float summonLoopTimer_ = 0.0f;

	// 1召喚ループの時間
	float summonLoopTime_ = 0.5f;

	// 範囲攻撃クラスをすでに呼び出したか
	bool isAreaAttacked_ = false;

	Math::Vector3 lightningAttackOffsets_[2] = {
	    {2.5f,  2.5f, 0.0f},
        {-2.5f, 2.5f, 0.0f}
    };

	// 体力バースプライト
	KamataEngine::Sprite* healthSprite_ = nullptr;

	// 体力バー長さ
	static inline const float healthBarWidth_ = 512.0f;

	// 移動先座標
	std::vector<Math::Vector3> movementPositions_;

	// 目標移動先座標
	Math::Vector3 currentTargetPosition_ = {};

	// 行動タイマー
	float actionTimer_ = 0.0f;

	// 現在の行動
	Actions currentAction_ = Move;

	// 移動最大時間
	static inline const float moveActionMaxTime_ = 1.5f;

	// 乱数で指定する攻撃時間
	float attackLimitTime_ = 0.0f;

	// 攻撃最短時間
	float attackMinTime_ = 1.0f;

	// 攻撃最大時間
	float attackMaxTime_ = 5.0f;

	//------------------------------------
	//

	// 死亡演出タイマー
	float deathTimer_ = 0.0f;

	// 死亡演出時間
	float deathTime_ = 5.0f;

	// 死亡状態
	bool isDeath_ = false;

	// 死亡演出終了状態
	bool isDeathFinished_ = false;

	//
	//------------------------------------

	bool isSpawnSoundPlayed_ = false;
	bool isScreamSoundPlayed_ = false;

	// 出現タイマー
	float appearTimer_ = 0.0f;

	// ダメージリアクション時間<フレーム>
	uint32_t damageReactionTimer_ = 0;
	uint32_t damageReactionTime_ = 5;

	// カメラコントローラ
	CameraController* cameraController_ = nullptr;

public:
	~Boss();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// UI描画
	void DrawUI();

	bool IsDeathFinished() const { return isDeathFinished_; }

	Atrum::Entity* GetEntity() const { return entity_.get(); }
	std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }
	bool GetIsDeath() const { return isDeath_; }

	void SetCameraController(CameraController* control) { cameraController_ = control; }

};

} // namespace Atrum
