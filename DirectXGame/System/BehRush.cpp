#include "../System/BehRush.h"
#include "../Easing.h"
#include "../System/BehEnemyShot.h"
#include "../System/CmpEnemy.h"
#include "../System/Entity.h"
#include "../Time/DeltaTime.h"

namespace Atrum {

void BehRush::ResolveDependence() {
	frameDeltaTime_ = FrameDeltaTime::GetInstance();
	cmpEnemy_ = RefGrandOwner().GetUpdCmp<CmpEnemy>();
}

void BehRush::Initialize() { 
	sourcePos_ = RefGrandOwner().GetWorldPosition();
}

void BehRush::Execute() {

	timer_ += frameDeltaTime_->GetDeltaTime();

	if (timer_ >= endTime_) {

		timer_ = endTime_;
	}

	RefGrandOwner().RefTransform().translate = Interpolation::Lerp(sourcePos_, destinationPos_, Interpolation::EaseInBack(timer_ / endTime_));

	if (timer_ >= endTime_) {
		cmpEnemy_->RefBehaviorBox().Transition(new BehEnemyShot());
	}
}

} // namespace Atrum