#pragma once

#include "../System/BehaviorBox.h"
#include "../System/UpdComponent.h"

namespace Atrum {

class CmpReflectableBullet;
class Entity;

class CmpEnemy : public UpdComponent {
private:
	Entity* player_ = nullptr;
	BehaviorBox behaviorBox_{};

	uint64_t behaviorTransitionCountForShotMeteor_ = 10;
	uint64_t behaviorTransitionCountForWallShot_ = 6;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::INPUT; }

	void ResolveDependence() override;

	void Initialize() override;

	void Update() override;

	BehaviorBox& RefBehaviorBox() { return behaviorBox_; }
	Entity* GetPlayer() { return player_; }
	void SetPlayer(Entity* player) { player_ = player; }
};

} // namespace Atrum