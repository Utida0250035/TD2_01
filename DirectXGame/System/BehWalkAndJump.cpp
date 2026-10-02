#include "../System/BehWalkAndJump.h"
#include "../ConstantVal.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"
#include "../System/CmpDirection.h"
#include "../System/CmpLhRigidBody.h"
#include "../System/Playable/CmpInput.h"
#include <numbers>

namespace Atrum::Player {

void BehWalkAndJump::ResolveDependence() {
	cmpInput_ = CmpInput::GetInstance();

	cmpLhRigidBody_ = RefGrandOwner().GetUpdCmp<CmpLhRigidBody>();
	cmpDirection_ = RefGrandOwner().GetDataCmp<CmpDirection>();
}

void BehWalkAndJump::Execute() {

	Atrum::Math::Vector3& velocity = cmpLhRigidBody_->RefVelocity();

	const InputData& inputData = cmpInput_->GetData();

	Atrum::Math::Vector3& direction = cmpDirection_->RefDirection();

	Atrum::Math::Quaternion& quaternion = RefGrandOwner().RefTransform().quaternion;

	velocity.x = 0.0f;

	if (inputData.vec2Input.x < 0.0f) {
		velocity.x = -walkSpeed_;
		direction.x = -1.0f;
		quaternion = Math::Quaternion::FromAxisAngle(Math::Vector3::UpLh(), -std::numbers::pi_v<float> * 0.5f);
	} else if (inputData.vec2Input.x > 0.0f) {
		velocity.x = walkSpeed_;
		direction.x = 1.0f;
		quaternion = Math::Quaternion::FromAxisAngle(Math::Vector3::UpLh(), std::numbers::pi_v<float> * 0.5f);
	}

	if (inputData.isJumpTrigger) {
		if (RefGrandOwner().GetTransform().translate.y <= kGroundHeight) {

			isAirJumpVariable_ = true;

			velocity.y = jumpSpeedOrigin_;
		} else {
			if (isAirJumpVariable_) {
				velocity.y = jumpSpeedOrigin_;
				isAirJumpVariable_ = false;
			}
		}
	}
}

} // namespace Atrum::Player