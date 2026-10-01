#pragma once

#include "KamataEngine.h"

/// <summary>
/// 天球
/// </summary>
class Skydome {

private:

	/// <summary>
	/// ワールド変換
	/// </summary>
	KamataEngine::WorldTransform worldTransform_;

	/// <summary>
	/// 3Dモデル
	/// </summary>
	KamataEngine::Model* model_ = nullptr;

	/// <summary>
	/// カメラ
	/// </summary>
	KamataEngine::Camera* camera_ = nullptr;

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

	~Skydome() {

		if (model_) {
		
			delete model_;
			model_ = nullptr;
		
		}

	}

};