#include "../System/BehEnemyWallShot.h"
#include "../Audio/Audio.h"
#include "../System/BehRush.h"
#include "../System/CmpEnemy.h"
#include "../System/CmpReflectableBullet.h"
#include "../System/EntityFactory.h"
#include "../Time/DeltaTime.h"

namespace Atrum {

void BehEnemyWallShot::ResolveDependence() {
	cmpBullet_ = RefGrandOwner().GetUpdCmp<CmpReflectableBullet>();
	cmpEnemy_ = RefGrandOwner().GetUpdCmp<CmpEnemy>();

	frameDeltaTime_ = FrameDeltaTime::GetInstance();
}

void BehEnemyWallShot::Initialize() { shotCoolTimer_ = 0.0f; }

void BehEnemyWallShot::Execute() {

	if (shotCoolTimer_ <= 0.0f) {

		float x = 0.5f + kLeftEndX;

		std::vector<float> rapidBulletShotX = {};

		while (x < kRightEndX) {
			rapidBulletShotX.push_back(x);
			x += 6.0f;
		}

		for (auto& shotX : rapidBulletShotX) {
			cmpBullet_->Shot(EntityFactory::LinearEnemyBullet({0.0f, -rapidBulletSpeed_, 0.0f}, {shotX, bulletShotY_, 0.0f}).release());
		}

		Audio::Manager::GetInstance()->Play("./Resources/seEnemyShot.mp3");

		x = 0.5f + kLeftEndX;

		std::vector<float> slowBulletShotX = {};

		while (x < kRightEndX) {
			slowBulletShotX.push_back(x);
			x += 8.0f;
		}

		slowBulletShotX.erase(slowBulletShotX.begin() + shotCount_ * 2ULL);

		for (const auto& shotX : slowBulletShotX) {
			cmpBullet_->Shot(EntityFactory::LinearEnemyBullet({0.0f, -slowBulletSpeed_, 0.0f}, {shotX, bulletShotY_, 0.0f}).release());
		}

		Audio::Manager::GetInstance()->Play("./Resources/seEnemyShot.mp3");

		shotCount_++;
		shotCoolTimer_ = shotCoolTime_;

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