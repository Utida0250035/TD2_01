#pragma once

#include "../System/DrawComponent.h"

namespace Atrum {

class CmpReflectableBullet;

class CmpDrawBullet : public DrawComponent {
private:
	CmpReflectableBullet* cmpBullet_ = nullptr;

public:
	void ResolveDependence() override;
	void Draw() override;
};

} // namespace Atrum