#include "../System/CmpOrbiter.h"
#include "../Math/Quaternion.h"
#include "../System/Entity.h"
#include "../Time/DeltaTime.h"
#include <cmath>

#include <iostream>

namespace Atrum {

void CmpOrbiter::ResolveDependence() { frameDeltaTime_ = FrameDeltaTime::GetInstance(); }

void CmpOrbiter::Update() {

	Math::Quaternion deltaQ = Math::Quaternion::FromAxisAngle(rotateAxis_, rotateSpeed_ * frameDeltaTime_->GetDeltaTime());

	Math::Vector3& translate = RefOwner().RefTransform().translate;

	translate = deltaQ.RotateVector(translate);

}

} // namespace Atrum