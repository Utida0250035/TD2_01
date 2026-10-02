#pragma once

#include "../Math/Vector3.h"
#include "../System/DataComponent.h"
#include "../System/UpdComponent.h"

namespace Atrum {

class FrameDeltaTime;

class CmpOrbiter : public UpdComponent {
private:
	Math::Vector3 rotateAxis_ = Math::Vector3::UpLh();
	float rotateSpeed_ = 0.0f;
	FrameDeltaTime* frameDeltaTime_ = nullptr;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::MOVEMENT; }

	void ResolveDependence() override;

	void Update() override;

	void SetRotateAxis(const Math::Vector3& axis) { rotateAxis_ = axis; }
	void SetRotateSpeed(const float rotateSpeed) { rotateSpeed_ = rotateSpeed; }
};

} // namespace Atrum