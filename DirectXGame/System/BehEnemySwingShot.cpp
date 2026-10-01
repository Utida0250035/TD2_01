#include "../System/BehEnemySwingShot.h"
#include "../System/BehRush.h"
#include "../System/CmpEnemy.h"
#include "../System/CmpReflectableBullet.h"
#include "../System/CmpRotator.h"
#include "../System/EntityFactory.h"
#include "../Time/DeltaTime.h"

namespace Atrum {

void BehEnemyWallShot::ResolveDependence() {
	cmpBullet_ = RefGrandOwner().GetUpdCmp<CmpReflectableBullet>();
	cmpEnemy_ = RefGrandOwner().GetUpdCmp<CmpEnemy>();
	cmpRotator_ = RefGrandOwner().GetUpdCmp<CmpRotator>();

	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

void BehEnemyWallShot::Initialize() { shotCoolTimer_ = 0.0f; }

void BehEnemyWallShot::Execute() {

	if (shotCoolTimer_ <= 0.0f) {

		shotCoolTimer_ = shotCoolTime_;

		std::unique_ptr<Entity> entBullet = EntityFactory::LinearEnemyBullet({rapidBulletSpeed_, 0.0f, 0.0f}, {});

		entBullet->SetParent(&cmpRotator_->RefPivot());
		cmpBullet_->Shot(entBullet.release());
		shotCount_++;

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