#include "CameraController.h"
#include "./Time/DeltaTime.h"
#include "./Math/Lerp.h"
#include "Player.h"
#include <algorithm>

namespace {

namespace I = Atrum::Interpolation;
namespace A = Atrum;

} // namespace

void CameraController::Initialize(KamataEngine::Camera* camera) { camera_ = camera; }

void CameraController::CameraReset() { camera_->translation_ = target_->GetWorldTransform().translation_ + ToKamataEngine(targetOffset_); }

void CameraController::Update() {

#ifdef USE_IMGUI

	ImGui::Begin("cameracontroller");

	ImGui::SmallButton("modeChange");

	if (ImGui::IsItemActivated()) {

		if (mode_ == Mode::kFollow) {

			mode_ = Mode::kForcedScroll;
		} else {

			mode_ = Mode::kFollow;
		}
	}

	const char* text[2] = {"Follow", "forceScroll"};

	ImGui::Text(text[static_cast<int>(mode_)]);

	ImGui::End();

#endif

	if (!target_->GetIsAlive()) {

		return;
	}

	if (mode_ == Mode::kFollow) {

		followPosition_ = FromKamataEngine(target_->GetWorldTransform().translation_) + targetOffset_ + target_->GetVelocity() * kVelocityBias_;

		camera_->translation_.x = I::Lerp(camera_->translation_.x, followPosition_.x, kFollowRate_);
		camera_->translation_.y = I::Lerp(camera_->translation_.y, followPosition_.y, kFollowRate_);

		camera_->translation_.x =
		    std::clamp(camera_->translation_.x, target_->GetWorldTransform().translation_.x + followMergin_.left, target_->GetWorldTransform().translation_.x + followMergin_.right);

		camera_->translation_.y =
		    std::clamp(camera_->translation_.y, target_->GetWorldTransform().translation_.y + followMergin_.bottom, target_->GetWorldTransform().translation_.y + followMergin_.top);

	} else {

		followPosition_ += {forceScrollSpeed_ * A::FrameDeltaTime::GetInstance()->GetDeltaTime(), 0.0f, 0.0f};

		camera_->translation_.x = I::Lerp(camera_->translation_.x, followPosition_.x, kFollowRate_);
		camera_->translation_.y = I::Lerp(camera_->translation_.y, followPosition_.y, kFollowRate_);
	}

	camera_->translation_.x = std::clamp(camera_->translation_.x, movableArea_.left, movableArea_.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, movableArea_.bottom, movableArea_.top);

	camera_->UpdateMatrix();
}