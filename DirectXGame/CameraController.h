#pragma once
#include "KamataEngine.h"
#include "Math/AeVector3.h"

// 前方宣言 自機クラス
class Player;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {

public:

	enum class Mode {
		kFollow,
		kForcedScroll
	};

private:

	Mode mode_ = Mode::kFollow;

	float forceScrollSpeed_ = 1.0f;

	/// <summary>
	/// カメラ
	/// </summary>
	KamataEngine::Camera* camera_ = nullptr;

	/// <summary>
	/// 追従対象
	/// </summary>
	Player* target_ = nullptr;

	/// <summary>
	/// 追従対象とカメラの座標の差
	/// </summary>
	Atrum::Math::Vector3 targetOffset_ = {0.0f, 0.0f, -30.0f};

	/// <summary>
	/// 範囲定義用
	/// </summary>
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	/// <summary>
	/// 追従範囲
	/// </summary>
	Rect movableArea_ = {0.0f, 100.0f, 0.0f, 100.0f};

	/// <summary>
	/// 追従位置
	/// </summary>
	Atrum::Math::Vector3 followPosition_ = {};

	/// <summary>
	/// 追従率(追従強度)
	/// </summary>
	static inline const float kFollowRate_ = 0.125f;

	/// <summary>
	/// 先撮り倍率
	/// </summary>
	static inline const float kVelocityBias_ = 0.5f;

	/// <summary>
	/// 追従マージン
	/// </summary>
	static inline const Rect followMergin_ = Rect{-16.0f, 16.0f, -16.0f, 16.0f};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Camera* camera);

	/// <summary>
	/// リセット
	/// </summary>
	void CameraReset();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/* ゲッター */

	Mode GetMode() const { return mode_; }

	/* セッター */

	void SetMode(const Mode mode) { mode_ = mode; }

	/// <summary>
	///
	/// </summary>
	/// <param name="target"> 追従対象のインスタンス </param>
	void SetTarget(Player* target) { target_ = target; }

	void SetTargetOffset(const Atrum::Math::Vector3& offset) { targetOffset_ = offset; }

	/// <summary>
	///
	/// </summary>
	/// <param name="area"> 追従範囲 Left,Right,Bottom,Top </param>
	void SetMovableArea(Rect area) { movableArea_ = area; }
};