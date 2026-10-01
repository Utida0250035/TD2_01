#include "../System/BehEnemyShot.h"
#include "../System/BehRush.h"
#include "../System/CmpEnemy.h"
#include "../System/CmpReflectableBullet.h"
#include "../System/EntityFactory.h"
#include "../Time/DeltaTime.h"
#include "../Audio/Audio.h"

namespace Atrum {

void BehEnemyShot::ResolveDependence() {
	cmpBullet_ = RefGrandOwner().GetUpdCmp<CmpReflectableBullet>();
	cmpEnemy_ = RefGrandOwner().GetUpdCmp<CmpEnemy>();

	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

void BehEnemyShot::Initialize() { shotCoolTimer_ = 0.0f; }

void BehEnemyShot::Execute() {

	if (shotCoolTimer_ <= 0.0f) {

		shotCoolTimer_ = shotCoolTime_;
		cmpBullet_->Shot(
		    EntityFactory::LinearEnemyBullet((cmpEnemy_->GetPlayer()->GetWorldPosition() - RefGrandOwner().GetWorldPosition()).Normalized() * bulletSpeed_, RefGrandOwner().GetTransform().translate)
		        .release());
		shotCount_++;

			Audio::Manager::GetInstance()->Play("./Resources/seEnemyShot.mp3");

	} else {

		shotCoolTimer_ -= frameDeltaTime_->GetDeltaTime();
		if (shotCoolTimer_ <= 0.0f) {
			shotCoolTimer_ = 0.0f;
		}
	}

	if (shotCount_ >= shotCountMax_) {

		shotCount_ = shotCountMax_;

		BehRush* behRush = new BehRush();
		behRush->SetDestinationPos(cmpEnemy_->GetPlayer()->GetWorldPosition());

		cmpEnemy_->RefBehaviorBox().Transition(behRush);
	}
}

} // namespace Atrum