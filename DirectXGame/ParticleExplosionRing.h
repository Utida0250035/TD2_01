#pragma once

#include "KamataEngine.h"

#include "System/Entity.h"
#include "Particle/Particle.h"

namespace Atrum {

// 爆風パーティクル
class ParticleExplosion final : public Particles {

private:

	//---------------------------------
	//

	#pragma region

	//-----------------
	// 火

	#pragma region

	// 火の大きさ線形補間割合
	static inline const float fireSizeLerp_ = 0.8f;

	// 火の目標大きさ
	static inline const float fireTargetSize_ = 20.0f;

	// 火のアルファ値線形補間割合
	static inline const float fireAlphaLerp_ = 0.5f;

	#pragma endregion

	//
	//-----------------

	//-----------------
	// 煙

	#pragma region

	// 煙の大きさ線形補間割合
	static inline const float smokeSizeLerp_ = 0.1f;

	// 煙の目標大きさ
	static inline const float smokeTargetSize_ = 30.0f;

	// 煙のアルファ値線形補間割合
	static inline const float smokeAlphaLerp_ = 0.2f;

	#pragma endregion

	//
	//-----------------

	//-----------------
	// リング

	#pragma region

	// 煙の大きさ線形補間割合
	static inline const float ringSizeLerp_ = 0.1f;

	// 煙の目標大きさ
	static inline const float ringTargetSize_ = 50.0f;

	// 煙のアルファ値線形補間割合
	static inline const float ringAlphaLerp_ = 0.5f;

	#pragma endregion

	//
	//-----------------

	#pragma endregion

	//
	//---------------------------------

	//---------------------------------
	// 構造体

	// 爆発
	struct ExplosionStruct {

		// モデル
		KamataEngine::Model* model_ = nullptr;

		// ワールド変換データ
		KamataEngine::WorldTransform worldTransform_;

		// オブジェクトカラー
		KamataEngine::ObjectColor objectColor_;

		// アルファ値
		float alpha_ = 1.0f;

	};

	// 火
	ExplosionStruct fire_{};

	// 煙
	ExplosionStruct smoke_{};

	// リング
	ExplosionStruct ring_{};

	//
	//---------------------------------

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 初期座標
	KamataEngine::Vector3 initialTranslation_ = {};

	float Lerp(const float start, const float end, const float t) { return ((1.0f - t) * start) + (t * end);}

public:

	// デストラクタ
	~ParticleExplosion();

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 終了
	void Finalize() override;

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
	/// <param name="fireModel">火モデル</param>
	/// <param name="smokeModel">煙モデル</param>
	/// <param name="ringModel">リングモデル</param>
	void SetModel(KamataEngine::Model* fireModel, KamataEngine::Model* smokeModel, KamataEngine::Model* ringModel);

};

} // namespace Atrum