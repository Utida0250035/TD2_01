#pragma once

#include "../System/EntityType.h"
#include "../System/UpdComponent.h"

#include <memory>
#include <vector>

namespace Atrum {

class Entity;

class CmpReflectableBullet : public UpdComponent {
private:
	std::vector<std::unique_ptr<Entity>> reflectableBullets_{};

	EntityType reflecterType_ = EntityType::PLAYER_WEAPON;

	inline static constexpr float kSpaceTopBuffer = 8.0f;
	inline static constexpr float kSpaceSideBuffer = 8.0f;
	inline static constexpr float kSpaceBottomBuffer = 0.25f;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::MOVEMENT; }

	void Initialize() override;
	void Update() override;

	void Shot(Entity* bullet);

	std::vector<std::unique_ptr<Entity>>& RefBullets() { return reflectableBullets_; }
};

} // namespace Atrum