#pragma once

#include <memory>

#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../System/BehaviorBox.h"
#include "../../System/Playable/CmpInput.h"
#include "../../System/UpdComponent.h"

namespace Atrum {

class CmpHitSphere;
class FrameDeltatime;
class CmpSwing;
class CmpMesh;

} // namespace Atrum

namespace Atrum::Player {

class CmpInput;

class CmpMoveByInput2D : public UpdComponent {
private:
	CmpInput* cmpInput_ = nullptr;
	InputData inputData_{};

	FrameDeltaTime* frameDeltaTime_ = nullptr;

	BehaviorBox behaviorBox_{};

	CmpHitSphere* parryCmpHitSphere_ = nullptr;
	CmpHitSphere* cmpHitSphere_ = nullptr;

	CmpSwing* cmpSwingWeapon_ = nullptr;
	CmpMesh* cmpMesh_ = nullptr;

	Math::Vector4 invincibleColor = {0.75f, 0.75f, 0.1f, 1.0f};
	bool isInvincibleColorWaitNextFrame_ = false;

	float dodgeCoolTimer_ = 0.0f;
	float dodgeCoolTime_ = 0.5f;

	float parryDuration_ = 0.25f;
	float parryTimer_ = 0.0f;

	float invincibleDuration_ = 0.03125f;
	float invincibleTimer_ = 0.0f;

	bool isAirJumpVariable_ = false;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::MOVEMENT; }

	void ResolveDependence() override;
	void Initialize() override;
	void Update() override;

	BehaviorBox& RefBehaviorBox() { return behaviorBox_; }
};

} // namespace Atrum::Player