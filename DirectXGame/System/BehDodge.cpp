#include "../System/BehDodge.h"

#include "../Math/AeVector3.h"
#include "../System/BehaviorBox.h"
#include "../System/CmpDirection.h"
#include "../System/CmpLhRigidBody.h"
#include "../Time/DeltaTime.h"


namespace Atrum {

void BehDodge::Initialize() {
	cmpLhRigidBody_->RefVelocity() = {};
	dodgeTimer_ = dodgeTime_;
}

void BehDodge::Execute() {
	if (isFinished_) {
		return;
	}

	Math::Vector3& velocity = cmpLhRigidBody_->RefVelocity();

	velocity = cmpDirection_->GetDirection() * dodgeSpeed_;

	dodgeTimer_ -= frameDeltaTime_->GetDeltaTime();

	if (dodgeTimer_ <= 0.0f) {
		dodgeTimer_ = 0.0f;
		isFinished_ = true;

		pBehaviorBox_->Transition(nextBehavior_.release());
	}
}

void BehDodge::Finalize() {
	Math::Vector3& velocity = cmpLhRigidBody_->RefVelocity();
	velocity = {};
}

void BehDodge::ResolveDependence() {

	cmpDirection_ = RefGrandOwner().GetDataCmp<CmpDirection>();
	cmpLhRigidBody_ = RefGrandOwner().GetUpdCmp<CmpLhRigidBody>();
	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

} // namespace Atrum