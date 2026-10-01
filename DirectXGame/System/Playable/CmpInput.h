#pragma once

#include "../Math/AeVector2.h"
#include "../UpdComponent.h"
#include <KamataEngine.h>
#include <cstdint>
#include <unordered_map>

namespace Atrum {

class FrameDeltaTime;

}

namespace Atrum::Player {

// プレイヤーの入力抽象データ
struct InputData {
	Math::Vector2 vec2Input{};

	bool isJumpTrigger = false;
	bool isJumpPress = false;
	bool isJumpRelease = false;

	bool isAttackTrigger = false;
	bool isAttackPress = false;
	bool isAttackRelease = false;

	bool isDodgeTrigger = false;
	bool isDashPress = false;
};

class CmpInput : public UpdComponent {
private:
	// プレイヤー入力
	KamataEngine::Input* input_ = nullptr;

	// プレイヤー入力抽象データ
	InputData inputData_{};

	// フレーム間の時間差分クラス
	FrameDeltaTime* frameDeltaTime_ = nullptr;
	// 回避の入力猶予タイマー
	float dodgeBufferTimer_ = 0.0f;
	// 回避の入力猶予時間
	float dodgeBufferTime_ = 0.25f;

	float minThresholdVec2InputLengthSq_ = 0.25f;
	float maxThresholdVec2InputLengthSq_ = 0.64f;
	float thresholdDotSameDirection_ = 0.9f;

	bool isWaitingVec2InputRelease_ = false;

	float dashStopTimer_ = 0.0f;
	float dashStopTime_ = 0.25f;

	// ダッシュ入力フラグ
	bool isDashPress_ = false;

	bool isJumpKeyPrePush_ = false;

	Atrum::Math::Vector2 vec2InputBuffer_{};

	CmpInput() = default;
	~CmpInput() = default;

public:
	// コンポーネントの分類
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::INPUT; }

	// 依存関係の解決
	void ResolveDependence() override;
	// 実行
	void Update() override;
	// 入力データの取得
	const InputData& GetData() const { return inputData_; }

	static CmpInput* GetInstance() {
		static CmpInput instance;
		return &instance;
	}

	CmpInput(const CmpInput& osurce) = delete;
	CmpInput operator=(const CmpInput& source) = delete;
};

} // namespace Atrum::Player