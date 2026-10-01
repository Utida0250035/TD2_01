#include "../System/CmpDrawBullet.h"
#include "../System/CmpReflectableBullet.h"
#include "../System/Entity.h"

namespace Atrum {

void CmpDrawBullet::ResolveDependence() { cmpBullet_ = RefOwner().GetUpdCmp<CmpReflectableBullet>(); }

void CmpDrawBullet::Draw() {

	std::vector<std::unique_ptr<Entity>>& bullets = cmpBullet_->RefBullets();

	for (auto& bullet : bullets) {

		bullet->Draw();
	}
}

} // namespace Atrum