#pragma once

#include "../Math/Vector3.h"
#include <memory>
#include <vector>

class Player;

namespace Atrum {

class Entity;

class EntityFactory final {
private:
	// コンストラクタの削除と隠ぺい
	EntityFactory() = delete;

public:
	static std::unique_ptr<Entity> PlayerMononofu();
	static std::vector<std::unique_ptr<Entity>> ScrollSpheres();
	static std::unique_ptr<Entity> LinearEnemyBullet(const Math::Vector3& velocity, const Math::Vector3& position);
	static std::unique_ptr<Entity> Parry(Entity* parent);
	static std::unique_ptr<Entity> Boom(const Math::Vector3& pos, ::Player *player = nullptr);
	static std::unique_ptr<Entity> BigBoom(const Math::Vector3& pos, ::Player* player = nullptr);


	// コピーコンストラクタの削除
	EntityFactory(const EntityFactory& source) = delete;
	// 代入演算子の削除
	EntityFactory operator=(const EntityFactory& source) = delete;
};

} // namespace Atrum