#pragma once

#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <vector>

namespace Atrum {

// 粒子個々の情報
struct ParticleInfo {
	// 色(RGBA)
	Math::Vector4 color = Math::Vector4::White();
	// 位置
	Math::Vector3 position{};
	// 速度
	Math::Vector3 velocity{};
	// 加速度
	Math::Vector3 acceleration{};
	// 寿命設定
	float lifeTime_ = 0.0f;
	// 寿命タイマー
	float lifeTimer_ = 0.0f;
};

class Particles {

protected:
	// パーティクルの終了フラグ
	bool isFinish_;

	// 各粒子の情報
	std::vector<ParticleInfo> particleInfo_{};

public:
	virtual void Initialize() {}
	virtual void Update() {}
	virtual void Finalize() {}
	virtual void Draw() {}

	Particles();

	virtual ~Particles() {}

	bool GetIsFinish() const { return isFinish_; }
};

} // namespace Atrum