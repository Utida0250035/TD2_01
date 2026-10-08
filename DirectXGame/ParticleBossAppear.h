#pragma once

#include "KamataEngine.h"

#include "Particle/Particle.h"

namespace Atrum {



// ボス登場パーティクル
class ParticleBossAppear : public Particles {

private:

	// このクラス用のパーティクルデータ
	struct ParticleData {

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

	};

	// 半径
	float radius_ = 10.0f;

	// モデル
	KamataEngine::Model* model_ = nullptr;



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
};

} // namespace Atrum