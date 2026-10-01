#include "../System/CmpRotator.h"
#include "../Math/Quaternion.h"
#include "../System/Entity.h"
#include "../Time/DeltaTime.h"
#include <cmath>

#include <iostream>

namespace Atrum {

void CmpRotator::ResolveDependence() { frameDeltaTime_ = FrameDeltaTime::GetInstance(); }

void CmpRotator::Update() {
	Math::Quaternion deltaQ = Math::Quaternion::FromAxisAngle(rotateAxis_, rotateSpeed_ * frameDeltaTime_->GetDeltaTime());

	Math::Quaternion& quaternion = entPivot_->RefTransform().quaternion;

	quaternion = deltaQ * quaternion;
}

} // namespace Atrum