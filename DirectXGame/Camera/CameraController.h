#pragma once
#include <KamataEngine.h>
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include "../System/Entity.h"
#include "../Camera/Camera.h"


/// <summary>
/// 追従カメラ
/// </summary>
class CameraController {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// オフセット計算
	/// </summary>
	Atrum::Math::Vector3 Offset() const;

	// ゲッター
	KamataEngine::Matrix4x4 GetView() { return view_; }
	KamataEngine::Matrix4x4 GetProjection() { return projection_; }

	// セッター
	void SetTarget(const Atrum::Entity* target);
	void SetCamera(Atrum::Camera* camera) { camera_ = camera; }

private:
	// カメラ
	Atrum::Camera* camera_ = nullptr;

	// 移動量
	Atrum::Math::Vector3 translate_;
	// 回転量
	Atrum::Math::Vector3 rotate_;

	// ビュー行列
	KamataEngine::Matrix4x4 view_;

	// プロジェクション行列
	KamataEngine::Matrix4x4 projection_;

	// 追従対象
	const Atrum::Entity* target_ = nullptr;

	// 追従対象の座標
	Atrum::Math::Vector3 interTarget_ = {};

	// オフセット
	Atrum::Math::Vector3 offset_ = {0.0f, 0.0f, -10.0f};

	// 目標角度
	float destinationAngleY_ = 0.0f;
	float goalAngleY_ = 0.0f;
	float destinationAngleX_ = 0.0f;
	float goalAngleX_ = 0.0f;


	// 回転スピード
	static inline const float kRotSpeed = 0.1f;
};

