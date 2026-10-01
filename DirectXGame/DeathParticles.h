#pragma once

#include "KamataEngine.h"
#include "Math/AeVector3.h"
#include "Math/AeVector4.h"
#include <array>

class DeathParticles {

private:

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	static inline constexpr size_t kParticlesCount_ = 8u;

	std::array<KamataEngine::WorldTransform, kParticlesCount_> worldTransforms_;

	// 存続時間
	static inline constexpr float kDuration_ = 3.0f;

	// 移動の速さ
	static inline constexpr float kSpeed_ = 1.0f;

	// 分割した1個分の角度 2pi / 8.0f
	static inline constexpr float kAngleUnit_ = 3.14159f * 2.0f * 0.125f;

	// 終了フラグ
	bool isFinished_ = false;

	// 経過時間カウント
	float lifeCount_ = 0.0f;

	// 色変更
	KamataEngine::ObjectColor objectColor_{};

	// 色
	Atrum::Math::Vector4 color_{};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"> 3Dモデル </param>
	/// <param name="textureHandle"> テクスチャハンドル </param>
	/// <param name="camera"> カメラ </param>
	void Initialize(KamataEngine::Model* model, const uint32_t& textureHandle, KamataEngine::Camera* camera, const Atrum::Math::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const float& deltaTime);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	/* ゲッター */

	bool GetIsFinished() const { return isFinished_; }

};