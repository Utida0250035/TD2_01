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

	// cameraに適用
	if (camera_) {
		camera_->translation_ = ToKamataEngine(translate_);
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

void CameraController::SetTarget(const Atrum::Math::Vector3& target) {
	target_ = target;
	//Reset();
}
