#pragma once

#include "KamataEngine.h"

#include "Particle/Particle.h"

namespace Atrum {

// ボス敗北(消滅)パーティクル
class ParticleBossDefeat : public Particles {

private:

	//---------------------------------------------
	// プロパティ

	// 初速
	float initialSpeed_ = 50.0f;

	// 初期サイズ
	float initialSize_ = 5.0f;

	// 生成できるか
	bool canCreate_ = true;

	//
	//---------------------------------------------

	//---------------------------------------------
	//

	// このクラス用のパーティクルデータ
	class ParticleData final {

	public:
		// ワールド変換データ
		KamataEngine::WorldTransform worldTransform_;

		// ベクトル
		Math::Vector3 velocity_ = {};

		// オブジェクトカラー
		KamataEngine::ObjectColor objectColor_;

		// アルファ値
		float alpha_ = 1.0f;

		// 寿命タイマー
		bool isFinish_ = false;
	};

	//
	//---------------------------------------------

	//---------------------------------------------
	//

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 初期座標
	Math::Vector3 initialPosition_ = {};

	// パーティクルリスト
	std::list<ParticleData*> particles_;

	float Lerp(const float start, const float end, const float t) { return ((1.0f - t) * start) + (t * end); }

	//
	//---------------------------------------------

public:

	//---------------------------------------------
	//

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 終了
	void Finalize() override;

	//
	//---------------------------------------------

	//---------------------------------------------
	//

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
	void SetInitialPosition(const Math::Vector3 position) { initialPosition_ = position; }

	//
	//---------------------------------------------

};

} // namespace Atrum