#include "../System/BehEnemyMeteor.h"
#include "../System/BehRush.h"
#include "../System/CmpEnemy.h"
#include "../System/CmpReflectableBullet.h"
#include "../System/EntityFactory.h"
#include "../Time/DeltaTime.h"
#include "../Audio/Audio.h"

namespace Atrum {

void BehEnemyMeteor::ResolveDependence() {
	cmpBullet_ = RefGrandOwner().GetUpdCmp<CmpReflectableBullet>();
	cmpEnemy_ = RefGrandOwner().GetUpdCmp<CmpEnemy>();
	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

void BehEnemyMeteor::Initialize() { shotCoolTimer_ = 0.0f; }

void BehEnemyMeteor::Execute() {

	if (shotCoolTimer_ <= 0.0f) {

		shotCoolTimer_ = shotCoolTime_;
		cmpBullet_->Shot(EntityFactory::LinearEnemyBullet({0.0f, -bulletSpeed_, 0.0f}, {cmpEnemy_->GetPlayer()->GetWorldPosition().x, bulletShotY_, 0.0f}).release());
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