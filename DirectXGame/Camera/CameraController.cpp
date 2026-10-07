#include "CameraController.h"
#include "../Math/Vector3.h"
#include "../KeVectorUtility.h"
#include "../KeMainCamera.h"
#include <algorithm>
#include <numbers>

using namespace Atrum::Math;

void CameraController::Initialize() {
	camera_ = KeMainCamera::GetInstance()->camera_;
	interTarget_ = firstPos_;

	// ランダム
	std::random_device seedGen;
	engine_.seed(seedGen());
}

void CameraController::Update() {
	// 追従対象がいれば
	if (isFollow_) {
		// 追従座標の補間
		interTarget_ = Atrum::Interpolation::Lerp(interTarget_, target_, 0.1f);

		// カメラ位置を計算
		Vector3 offset = Offset();
		translate_ = interTarget_ + offset;
	} else {
		target_ = firstPos_;

		// 追従座標の補間
		interTarget_ = Atrum::Interpolation::Lerp(interTarget_, target_, 0.1f);

		// カメラ位置を計算
		Vector3 offset = Offset();
		translate_ = interTarget_ + offset;
	}

	// シェイク
	if (shakeTimer_ > 0.0f) {
		if (amplitude_ > 0.0f) {
			std::uniform_real_distribution<float> distribution(-amplitude_, amplitude_);
			shake_ = {distribution(engine_), distribution(engine_), distribution(engine_)};
		}

		shakeTimer_ -= 1.0f / 60.0f;
		amplitude_ = maxAmplitude_ * (shakeTimer_ / shakeDuration_);
	} else {
		shake_ = {0.0f, 0.0f, 0.0f};
	}

	// cameraに適用
	if (camera_) {
		camera_->translation_ = ToKamataEngine(translate_ + shake_);
		camera_->UpdateMatrix();
		camera_->TransferMatrix();
	}
}

void CameraController::Reset() {
	// 追従対象がいるなら
	if (isFollow_) {
		// 追従座標・角度初期化
		interTarget_ = target_;
	}
	destinationAngleY_ = rotate_.y;

	// オフセット
	Vector3 offset = Offset();
	translate_ = interTarget_ + offset;
}

Vector3 CameraController::Offset() const {
	// オフセット
	Vector3 offset = offset_;

	// オフセットを回転
	Matrix4x4 rotateMatrixX = Matrix4x4::RotateX(rotate_.x);
	Matrix4x4 rotateMatrixY = Matrix4x4::RotateY(rotate_.y);
	Matrix4x4 rotateMatrix = rotateMatrixX * rotateMatrixY;
	offset = rotateMatrix.TransformNormal(offset);

	return offset;
}

void CameraController::Shake(float shakeDuration, float maxAmplitude) {
	if (shakeDuration <= 0.0f) {
		return;
	}

	shakeDuration_ = shakeDuration;
	maxAmplitude_ = maxAmplitude;
	shakeTimer_ = shakeDuration_;
	amplitude_ = maxAmplitude_;
}

void CameraController::SetTarget(const Atrum::Math::Vector3& target) {
	target_ = target;
	//Reset();
}
