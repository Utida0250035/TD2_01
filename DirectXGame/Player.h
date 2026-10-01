#pragma once
#include "./Math/AeVector3.h"
#include "MapChipField.h"
#include "AABB.h"
#include "Easing.h"
#include "KeMatrix3D.h"
#include "KeVectorUtility.h"
#include <KamataEngine.h>

/// <summary>
/// 自機クラス
/// </summary>
class Player {

private:
	struct CollisionMapInfo {
		bool isCollisionCeil = false;
		bool isLanding = false;
		bool isCollisionWallLeft = false;
		bool isCollisionWallRight = false;
		Atrum::Math::Vector3 displacement;
	};

	bool isAlive_ = true;

	enum Corner { CORNER_RIGHT_BOTTOM, CORNER_LEFT_BOTTOM, CORNER_RIGHT_TOP, CORNER_LEFT_TOP, CORNER_NUM };

	/// <summary>
	/// ワールド座標変換
	/// </summary>
	KamataEngine::WorldTransform worldTransform_;

	/// <summary>
	/// 3Dモデル
	/// </summary>
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Model* attackModel_ = nullptr;
	KamataEngine::WorldTransform attackWorldTransform_{};

	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	uint32_t textureHandle_ = 0u;

	/// <summary>
	/// カメラ
	/// </summary>
	KamataEngine::Camera* camera_ = nullptr;

	/// <summary>
	/// 速度
	/// </summary>
	Atrum::Math::Vector3 velocity_ = {};

	/// <summary>
	/// 加速度(スカラー)
	/// </summary>
	static inline const float kAcceleration_ = 20.0f;

	/// <summary>
	/// 加速度
	/// </summary>
	Atrum::Math::Vector3 acceleration_ = {};

	/// <summary>
	/// 速度減衰率
	/// </summary>
	static inline const float kAttenuation_ = 0.125f;

	/// <summary>
	/// 速さ限界
	/// </summary>
	static inline const float kHorizontalSpeedLimit = 8.0f;

	/// <summary>
	/// 左右の向き
	/// </summary>
	enum class LRDirection { DIRECTION_LEFT, DIRECTION_RIGHT };

	/// <summary>
	/// 左右の向き
	/// </summary>
	LRDirection lrDirection_ = LRDirection::DIRECTION_RIGHT;

	/// <summary>
	/// 旋回開始時の角度
	/// </summary>
	float turnFirstRotationY_ = 0.0f;

	/// <summary>
	/// 旋回タイマー
	/// </summary>
	float turnTimer_ = 0.0f;

	/// <summary>
	/// 旋回時間[s]
	/// </summary>
	static inline const float kTimeTurn = 0.25f;

	/// <summary>
	/// 接地フラグ
	/// </summary>
	bool isLanding_ = true;

	/// <summary>
	/// 重力加速度 下方向
	/// </summary>
	static inline const float kGravityAcceleration = -15.0f;

	/// <summary>
	/// 最大落下速度 下方向
	/// </summary>
	static inline const float kLimitFallSpeed_ = 8.0f;

	/// <summary>
	/// ジャンプ初速 上方向
	/// </summary>
	static inline const float kJumpVelocityOrigin_ = 8.0f;

	/// <summary>
	/// 当たり判定の大きさ ヨコ
	/// </summary>
	static inline const float kHitWidth_ = 1.0f;

	/// <summary>
	/// 当たり判定の大きさ タテpo
	/// </summary>
	static inline const float kHitHeight_ = 1.0f;

	/// <summary>
	/// 地形との衝突時の余白
	/// </summary>
	static inline const float kCollisionBlank_ = 0.0625f;

	/// <summary>
	/// 着地している時のX速度減衰率
	/// </summary>
	static inline const float kAttenuationLanding_ = 0.125f;

	/// <summary>
	/// 壁に衝突している時のX速度減衰率
	/// </summary>
	static inline const float kAttenuationCollisionWall_ = 0.03125f;

	/// <summary>
	/// マップチップフィールド
	/// </summary>
	MapChipField* mapChipField_ = nullptr;

	Atrum::Math::Vector3 GetCornerPosition(const Atrum::Math::Vector3& center, const Corner& corner) {

		Atrum::Math::Vector3 offsetTable[CORNER_NUM] = {
		    {kHitWidth_ / 2.0f,  -kHitHeight_ / 2.0f, 0.0f},
		    {-kHitWidth_ / 2.0f, -kHitHeight_ / 2.0f, 0.0f},
		    {kHitWidth_ / 2.0f,  kHitHeight_ / 2.0f,  0.0f},
		    {-kHitWidth_ / 2.0f, kHitHeight_ / 2.0f,  0.0f},
		};

		return center + offsetTable[static_cast<uint32_t>(corner)];
	}

	void MoveInput(const float& deltaTime);

	void TopCollisionMap(CollisionMapInfo& info);

	void BottomCollisionMap(CollisionMapInfo& info);

	void RightCollisionMap(CollisionMapInfo& info);

	void LeftCollisionMap(CollisionMapInfo& info);

	void CollisionMap(CollisionMapInfo& info);

	void ApplyDisplacement(CollisionMapInfo& info);

	void CollisionCeil(CollisionMapInfo& info);

	void CheckLanding(const CollisionMapInfo& info);

	void CheckCollisionWall(const CollisionMapInfo& info);

	void ClampToCameraRange(const CollisionMapInfo& info);

	void ImGui();

	enum class Behavior { kUnknown, kRoot, kAttack };

	Behavior behavior_ = Behavior::kRoot;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	void BehaviorRootInitialize();
	void BehaviorRootUpdate(const float deltaTime);

	enum class AttackPhase { kFocus, kRush, kAfter };

	AttackPhase attackPhase_ = AttackPhase::kFocus;

	float attackTimeParameter_ = 0.0f;

	float focusTimeLimit_ = 0.25f;

	float rushTimeLimit_ = 0.5f;
	float rushAttackSpeed_ = 12.0f;

	float attackAfterTimeLimit_ = 0.25f;

	uint8_t attackEnergy_ = 0;
	uint8_t attackEnergyMax_ = 1;

	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate(const float deltaTime);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"> 3Dモデル </param>
	/// <param name="textureHandle"> テクスチャハンドル </param>
	/// <param name="camera"> カメラ </param>
	void Initialize(KamataEngine::Model* model, const uint32_t& textureHandle, KamataEngine::Model* attackModel, KamataEngine::Camera* camera, const Atrum::Math::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const float deltaTime);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Jump();

	/* ゲッター */

	/// <summary>
	///
	/// </summary>
	/// <returns> 座標変換データ </returns>
	KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	///
	/// </summary>
	/// <returns> 速度 </returns>
	Atrum::Math::Vector3 GetVelocity() const { return velocity_; }

	Atrum::Math::Vector3 GetPosWorld() const;

	AABB GetAABB() const;

	bool GetIsAlive() const { return isAlive_; };

	Atrum::Math::Vector3 GetWorldPosition() const {
		const auto& mat = worldTransform_.matWorld_;

		return Atrum::Math::Vector3{mat.m[3][0], mat.m[3][1], mat.m[3][2]};
	}

	bool isAttack() {

		if (behavior_ == Behavior::kAttack) {
			if (attackPhase_ != AttackPhase::kFocus) {

				return true;
			}
		}

		return false;
	}

	/* セッター */

	/// <summary>
	///
	/// </summary>
	/// <param name="mapChipField"> マップチップフィールド </param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void SetIsAlive(const bool isAlive) { isAlive_ = isAlive; }
};