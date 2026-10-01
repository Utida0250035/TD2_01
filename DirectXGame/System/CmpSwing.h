#pragma once

#include "../Math/AeVector3.h"
#include "../System/UpdComponent.h"

namespace Atrum {

class FrameDeltaTime;
class Entity;

class CmpSwing : public UpdComponent {
private:
	float swingSpeed_ = 0.0f;
	float swingDuration_ = 0.0f;
	float swingTimer_ = 0.0f;
	Math::Vector3 swingAxis_ = Math::Vector3::Right();
	Entity* pivot_ = nullptr;
	FrameDeltaTime* frameDeltaTime_ = nullptr;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::MOVEMENT; }

	void Update() override;
	void ResolveDependence() override;
	void StartSwing();

	void SetPivot(Entity* pivot) { pivot_ = pivot; }
	void SetSwingDuration(const float duration) { swingDuration_ = duration; }
	void SetSwingSpeed(const float swingSpeed) { swingSpeed_ = swingSpeed; }
};

} // namespace Atrum