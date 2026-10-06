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
	Atrum::Math::Vector3 GetOffset() const { return offset_; }
	bool GetIsFollow() const { return isFollow_; }

	// セッター
	void SetTarget(const Atrum::Math::Vector3& target);
	void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }
	void SetOffset(const Atrum::Math::Vector3& offset) { offset_ = offset; }
	void SetIsFollow(const bool& isFollow) { isFollow_ = isFollow; }

private:
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 移動量
	Atrum::Math::Vector3 translate_;
	// 回転量
	Atrum::Math::Vector3 rotate_;

	// 追従座標
	Atrum::Math::Vector3 target_ = {};

	// 追従対象の座標
	Atrum::Math::Vector3 interTarget_ = {};

	// オフセット
	Atrum::Math::Vector3 offset_ = {0.0f, 0.0f, -15.0f};

	// 目標角度
	float destinationAngleY_ = 0.0f;
	float goalAngleY_ = 0.0f;
	float destinationAngleX_ = 0.0f;
	float goalAngleX_ = 0.0f;

	// 追従するか
	bool isFollow_ = false;

	// 初期座標
	Atrum::Math::Vector3 firstPos_ = {0.0f, 0.0f, -50.0f};
};

