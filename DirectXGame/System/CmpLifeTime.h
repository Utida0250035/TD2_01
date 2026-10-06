#pragma once

#include "UpdComponent.h"

namespace Atrum {

class FrameDeltaTime;

class CmpLifeTime : public UpdComponent {

public:
	constexpr UpdCmpGroup UpdGroup() { return UpdCmpGroup::PRE_RENDER; }

	/// <summary>
	/// 依存解決用 オーナーエンティティからのコンポーネントのポインタ取得やシングルトンクラスのポインタ取得など
	/// </summary>
	void ResolveDependence() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private:
	FrameDeltaTime* frameDeltaTime_ = nullptr;

	float lifeTime_ = 0.0f;
	float lifeTimer_ = 0.0f;

public:

	void SetLifeTime(const float seconds) { lifeTime_ = seconds; }

};

} // namespace Atrum