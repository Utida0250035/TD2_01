#include "CameraController.h"
#include "../Math/Vector3.h"
#include <algorithm>
#include <numbers>

using namespace Atrum::Math;

void CameraController::Initialize() {
}

void CameraController::Update() {
	// 追従対象がいれば
	if (target_) {
		// 追従座標の補間
		interTarget_ = Atrum::Interpolation::Lerp(interTarget_, target_->GetWorldPosition(), 0.3f);

		// カメラ位置を計算
		Vector3 offset = Offset();
		translate_ = interTarget_ + offset;
	}

	// cameraに適用
	if (camera_) {
		camera_->AddTranslate(translate_);
	}
}

void CameraController::Reset() {
	// 追従対象がいるなら
	if (target_) {
		// 追従座標・角度初期化
		interTarget_ = target_->GetWorldPosition();
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

void CameraController::SetTarget(const Atrum::Entity* target) {
	target_ = target;
	Reset();
}
