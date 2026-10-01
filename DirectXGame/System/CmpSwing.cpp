#include "../System/CmpSwing.h"
#include "../Math/Quaternion.h"
#include "../System/Entity.h"
#include "../Time/DeltaTime.h"

namespace Atrum {

void CmpSwing::Update() {

	if (swingTimer_ > 0.0f) {

		Math::Quaternion& quaternion = pivot_->RefTransform().quaternion;

		quaternion = Math::Quaternion::FromAxisAngle(swingAxis_, swingSpeed_ * frameDeltaTime_->GetDeltaTime()) * quaternion;

		swingTimer_ -= frameDeltaTime_->GetDeltaTime();

		if (swingTimer_ <= 0.0f) {

			swingTimer_ = 0.0f;

			pivot_->RefTransform() = pivot_->GetInitialTransform();
		}
	}
}

void CmpSwing::ResolveDependence() { frameDeltaTime_ = FrameDeltaTime::GetInstance(); }

void CmpSwing::StartSwing() {
	pivot_->RefTransform() = pivot_->GetInitialTransform();
	swingTimer_ = swingDuration_;
}

} // namespace Atrum