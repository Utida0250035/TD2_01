#include "../System/Playable/CmpMoveByInput.h"

#include "../../Time/DeltaTime.h"
#include "../Audio/Audio.h"
#include "../ConstantVal.h"
#include "../System/BehDodge.h"
#include "../System/BehWalkAndJump.h"
#include "../System/CmpHitSphere.h"
#include "../System/CmpMesh.h"
#include "../System/CmpParry.h"
#include "../System/CmpSwing.h"
#include "../System/Playable/CmpInput.h"

#include <iostream>

namespace {

namespace M = ::Atrum::Math;

}

namespace Atrum::Player {

void CmpMoveByInput2D::ResolveDependence() {
	cmpInput_ = CmpInput::GetInstance();
	frameDeltaTime_ = FrameDeltaTime::GetInstance();
	CmpParry* cmpParry = RefOwner().GetDataCmp<CmpParry>();
	parryCmpHitSphere_ = cmpParry->RefEntParry().GetUpdCmp<CmpHitSphere>();
	cmpHitSphere_ = RefOwner().GetUpdCmp<CmpHitSphere>();
	cmpSwingWeapon_ = RefOwner().GetUpdCmp<CmpSwing>();
	cmpMesh_ = RefOwner().GetDrawCmp<CmpMesh>();
};

void CmpMoveByInput2D::Initialize() {
	behaviorBox_.SetGrandOwner(&RefOwner());
	behaviorBox_.Transition(new BehWalkAndJump());
	parryCmpHitSphere_->SetIsActive(false);
	parryTimer_ = 0.0f;
	invincibleTimer_ = 0.0f;
}

void CmpMoveByInput2D::Update() {
	inputData_ = cmpInput_->GetData();

	behaviorBox_.Execute();

	parryCmpHitSphere_->SetIsActive(false);
	cmpHitSphere_->SetIsActive(true);
	cmpMesh_->SetColor(Math::Vector4::White());

	if (inputData_.isJumpTrigger) {

		if (RefOwner().GetWorldPosition().y <= kGroundHeight) {

			parryTimer_ = parryDuration_;
			invincibleTimer_ = invincibleDuration_;
			isAirJumpVariable_ = true;
			isInvincibleColorWaitNextFrame_ = true;

			cmpSwingWeapon_->StartSwing();

			Audio::Manager::GetInstance()->Play(Audio::Manager::GetInstance()->Get("./Resources/seSwingWeapon.mp3"));

		} else {

			if (isAirJumpVariable_) {

				Audio::Manager::GetInstance()->Play(Audio::Manager::GetInstance()->Get("./Resources/seSwingWeapon.mp3"));

				cmpSwingWeapon_->StartSwing();

				invincibleTimer_ = invincibleDuration_;

				parryTimer_ = parryDuration_;

				isInvincibleColorWaitNextFrame_ = true;

				isAirJumpVariable_ = false;
			}
		}
	}

	if (parryTimer_ > 0.0f) {

		parryCmpHitSphere_->SetIsActive(true);

		parryTimer_ -= frameDeltaTime_->GetDeltaTime();

		if (parryTimer_ <= 0.0f) {

			parryTimer_ = 0.0f;
		}
	}

	if (invincibleTimer_ > 0.0f) {

		cmpHitSphere_->SetIsActive(false);

		if (isInvincibleColorWaitNextFrame_) {
			isInvincibleColorWaitNextFrame_ = false;
		} else {
			cmpMesh_->SetColor(invincibleColor);
		}

		invincibleTimer_ -= frameDeltaTime_->GetDeltaTime();

		if (invincibleTimer_ <= 0.0f) {

			invincibleTimer_ = 0.0f;
			cmpMesh_->SetColor(Math::Vector4::White());
		}
	}

	if (inputData_.isDodgeTrigger) {

		if (dodgeCoolTimer_ <= 0.0f) {

			BehDodge* behDodge = new BehDodge();
			behDodge->SetPBehaviorBox(&behaviorBox_);
			behDodge->SetNextBehavior(new BehWalkAndJump());
			behaviorBox_.Transition(behDodge);

			dodgeCoolTimer_ = dodgeCoolTime_;

			Audio::Manager::GetInstance()->Play(Audio::Manager::GetInstance()->Get("./Resources/seStep.mp3"));
		}
	}

	if (dodgeCoolTimer_ > 0.0f) {
		dodgeCoolTimer_ -= frameDeltaTime_->GetDeltaTime();
		if (dodgeCoolTimer_ <= 0.0f) {
			dodgeCoolTimer_ = 0.0f;
		}
	}

	Math::Vector3 pos = RefOwner().GetWorldPosition();
}

} // namespace Atrum::Player