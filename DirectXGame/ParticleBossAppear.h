#pragma once

#include "KamataEngine.h"

#include "Particle/Particle.h"

namespace Atrum {

// ボス登場パーティクル
class ParticleBossAppear : public Particles {

private:

	// このクラス用のパーティクルデータ
	class ParticleData final {

	public:

		// ワールド変換データ
		KamataEngine::WorldTransform worldTransform_;

		// ベクトル
		Math::Vector3 velocity_ = {};

		// 加速度
		Math::Vector3 acceleration_ = {};

		// オブジェクトカラー
		KamataEngine::ObjectColor objectColor_;

		// アルファ値
		float alpha_ = 0.0f;

		// 寿命タイマー
		bool isFinish_ = false;
	};

	// 半径
	float radius_ = 20.0f;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// ターゲット座標
	Math::Vector3 targetPosition_ = {};

	// パーティクルリスト
	std::list<ParticleData*> particles_;

public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 終了
	void Finalize() override;

	/// <summary>
	/// モデル指定
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(KamataEngine::Model* model) {
		assert(model && "No Model!");
		model_ = model;
	}

	/// <summary>
	/// ターゲット座標指定
	/// </summary>
	/// <param name="position">ターゲット座標</param>
	void SetTargetPosition(const Math::Vector3 position) { targetPosition_ = position; }
};

} // namespace Atrum