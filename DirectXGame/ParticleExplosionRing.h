#pragma once

#include "KamataEngine.h"

#include "System/Entity.h"
#include "Particle/Particle.h"

namespace Atrum {

// 爆風リングパーティクル
class ParticleExplosionRing final : public Particles {

private:

	//---------------------------------
	//

	static inline const float sizeLerp_ = 0.3f;
	static inline const float endSize_ = 20.0f;

	static inline const float alphaLerp_ = 0.4f;

	//
	//---------------------------------

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// オブジェクトカラー
	KamataEngine::ObjectColor objectColor_;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 初期座標
	KamataEngine::Vector3 initialTranslation_ = {};

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	float alpha_ = 1.0f;

	float Lerp(const float start, const float end, const float t) { return ((1.0f - t) * start) + (t * end);}

public:

	//------------------------------
	//

	// デストラクタ
	~ParticleExplosionRing();

	/// <summary>
	/// 初期座標指定 構造上Initialize()の前に置かないと反映されない
	/// </summary>
	/// <param name="translation">座標</param>
	void SetInitialTranslation(const Math::Vector3 translation) {
		initialTranslation_.x = translation.x;
		initialTranslation_.y = translation.y;
		initialTranslation_.z = translation.z;
	}

	/// <summary>
	/// モデル指定
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(KamataEngine::Model *model) { model_ = model; }

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 終了
	void Finalize() override;

	//
	//------------------------------

};

} // namespace Atrum