#include "../System/CmpEnemy.h"
#include "../System/BehEnemyMeteor.h"
#include "../System/BehEnemyShot.h"
#include "../System/BehEnemyWallShot.h"
#include "../System/CmpReflectableBullet.h"
#include "../System/EntityStorage.h"

namespace Atrum {

void CmpEnemy::ResolveDependence() { behaviorBox_.SetGrandOwner(&RefOwner()); }

void CmpEnemy::Initialize() {
	behaviorBox_.Transition(new BehEnemyShot());
	behaviorBox_.InitializeTransitionCount();
}

void CmpEnemy::Update() {

	behaviorBox_.Execute();

	if ((behaviorBox_.GetTransitionCount() + 1) % behaviorTransitionCountForShotMeteor_ == 0) {
		behaviorBox_.Transition(new BehEnemyMeteor());
	} else if ((behaviorBox_.GetTransitionCount() + 1) % behaviorTransitionCountForWallShot_ == 0) {
		behaviorBox_.Transition(new BehEnemyWallShot());
	}
}

} // namespace Atrum