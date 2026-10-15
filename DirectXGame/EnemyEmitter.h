#pragma once

#include "Enemy.h"
#include "Math/Vector3.h"
#include <memory>
#include <vector>

namespace Atrum {

class EnemyEmitter {
public:
	static void Emit(std::vector<std::unique_ptr<Enemy>>& enemies, const Math::Vector3& bossPos);
};

} // namespace Atrum