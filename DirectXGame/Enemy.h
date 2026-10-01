#pragma once

#include "KamataEngine.h"
#include "MapChipField.h"
#include "KeMatrix3D.h"
#include "KeVectorUtility.h"
#include "./Math/AeVector3.h"
#include "AABB.h"

/// <summary>
/// 自機クラス
/// </summary>
class Enemy {

protected:
	
	struct CollisionMapInfo {
		bool isCollisionCeil = false;
		bool isLanding = false;
		bool isCollisionWall = false;
		Atrum::Math::Vector3 displacement;
	};

	enum Corner { CORNER_RIGHT_BOTTOM, CORNER_LEFT_BOTTOM, CORNER_RIGHT_TOP, CORNER_LEFT_TOP, CORNER_NUM };

	bool isAlive_ = true;
	bool isCollisionAble_ = true;
	float deathTimer_ = 0.0f;
	float timeForDeath_ = 0.75f;

	float deathShrinkSpeed_ = 3.0f;
	float deathRotateSpeed_ = 24.0f;

	/// <summary>
	/// ワールド座標変換
	/// </summary>
	KamataEngine::WorldTransform worldTransform_;

	/// <summary>
	/// 3Dモデル
	/// </summary>
	KamataEngine::Model* model_ = nullptr;

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
	static inline const float kAcceleration_ = 0.125f;

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
	static inline const float kHorizontalSpeedLimit = 2.0f;

	/// <summary>
	/// 左右の向き
	/// </summary>
	enum class LRDirection { DIRECTION_LEFT, DIRECTION_RIGHT };

	/// <summary>
	/// 左右の向き
	/// </summary>
	LRDirection lrDirection_ = LRDirection::DIRECTION_LEFT;

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
	static inline const float kTurnTime_ = 0.25f;

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
	/// 当たり判定の大きさ ヨコ
	/// </summary>
	static inline const float kHitWidth_ = 1.0f;

	/// <summary>
	/// 当たり判定の大きさ タテ
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
	/// 首振り歩きアニメーションの始めの角度
	/// </summary>
	static inline const float kWalkMotionAngleStart_ = 3.14159f * 0.5f;

	/// <summary>
	/// 首振り歩きアニメーションの終わりの角度
	/// </summary>
	static inline const float kWalkMotionAngleEnd_ = -3.14159f * 0.5f;

	/// <summary>
	/// 首振り歩きアニメーションの全体時間
	/// </summary>
	static inline const float kWalkMotionTime_ = 1.0f;

	/// <summary>
	/// 歩きモーションタイマー
	/// </summary>
	float walkMotionTimer_ = 0.0f;

	/// <summary>
	/// 往復にかける時間
	/// </summary>
	static inline const float kRoundTripTime_ = 10.0f;

	/// <summary>
	/// 往復タイマー
	/// </summary>
	float roundTripTimer_ = 0.0f;

	/// <summary>
	/// マップチップフィールド
	/// </summary>
	MapChipField* mapChipField_ = nullptr;

	Atrum::Math::Vector3 GetCornerPosition(const Atrum::Math::Vector3& center, const Corner& corner) {

		Atrum::Math::Vector3 offsetTable[CORNER_NUM] = {
		    {kHitWidth_ / 2.0f,  -kHitHeight_ / 2.0f, 0.0f},
		    {-kHitWidth_ / 2.0f, -kHitHeight_ / 2.0f, 0.0f},
		    {kHitWidth_ / 2.0f,  kHitHeight_ / 2.0f,  0.0f},
		    {-kHitWidth_ / 2.0f, kHitHeight_ / 2.0f, 0.0f},
		};

		return center + offsetTable[static_cast<uint32_t>(corner)];

	}

	virtual void Move(const float deltaTime);

	virtual void MoveMotion(const float deltaTime);

	virtual void MoveTurn(const float deltaTime);

	virtual void DeathMotion(const float deltaTime);

	void TopCollisionMap(CollisionMapInfo& info);

	void BottomCollisionMap(CollisionMapInfo& info);

	void RightCollisionMap(CollisionMapInfo& info);

	void LeftCollisionMap(CollisionMapInfo& info);

	void CollisionMap(CollisionMapInfo& info);

	void ApplyDisplacement(CollisionMapInfo& info);

	void CollisionCeil(CollisionMapInfo& info);

	void CheckLanding(const CollisionMapInfo& info);

	void CheckCollisionWall(const CollisionMapInfo& info);

	void ImGui();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"> 3Dモデル </param>
	/// <param name="textureHandle"> テクスチャハンドル </param>
	/// <param name="camera"> カメラ </param>
	void Initialize(KamataEngine::Model* model, const uint32_t& textureHandle, KamataEngine::Camera* camera, const Atrum::Math::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const float deltaTime);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// ゲッター

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

	Atrum::Math::Vector3 GetPosWorld() const { return Atrum::Math::Vector3{worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]}; }

	AABB GetAABB() const;

	bool GetIsAlive() const { return isAlive_; }
	bool GetIscollisionAble() const { return isCollisionAble_; }

	// セッター

	/// <summary>
	///
	/// </summary>
	/// <param name="mapChipField"> マップチップフィールド </param>
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void SetIsAlive(const bool isAlive) { isAlive_ = isAlive; };
	void SetIsCollisionAble(const bool isCollisionAble) { isCollisionAble_ = isCollisionAble; }

};