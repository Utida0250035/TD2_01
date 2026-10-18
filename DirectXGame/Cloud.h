#pragma once

#include "KamataEngine.h"

/// <summary>
/// 雲
/// </summary>
class Cloud {

private:
	static inline const int kCloudNum_ = 32;
	static inline const float kCloudspeed_ = 0.04f;

	/// <summary>
	/// ワールド変換
	/// </summary>
	KamataEngine::WorldTransform worldTransforms_[kCloudNum_];

	/// <summary>
	/// 3Dモデル
	/// </summary>
	KamataEngine::Model* model_ = nullptr;

	/// <summary>
	/// カメラ
	/// </summary>
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::ObjectColor color_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void InitializeCloud(KamataEngine::WorldTransform& worldTransform);

	~Cloud() {

		if (model_) {

			delete model_;
			model_ = nullptr;
		}
	}
};