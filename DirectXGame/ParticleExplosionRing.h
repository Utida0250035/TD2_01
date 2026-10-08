#pragma once

#include "System/Entity.h"
#include "Particle/Particle.h"

namespace Atrum {

// 爆風リングパーティクル
class ParticleExplosionRing final : public Particles {

private:

	// 描画用エンティティ
	std::unique_ptr<Entity> entity_ = nullptr;

	// s初期座標
	Math::Vector3 initialTranslation_ = {};

public:

	//------------------------------
	//

	// デストラクタ
	~ParticleExplosionRing();

	/// <summary>
	/// 初期座標指定 構造上Initialize()の前に置かないと反映されない
	/// </summary>
	/// <param name="translation">座標</param>
	void SetInitialTranslation(const Math::Vector3 translation) { initialTranslation_ = translation; }

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	void Finalize() override;

	//
	//------------------------------

};

} // namespace Atrum