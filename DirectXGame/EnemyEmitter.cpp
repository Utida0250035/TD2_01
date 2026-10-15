#include "EnemyEmitter.h"
#include "Random.h"
#include "Scene/SceneGame.h"

namespace Atrum {
void EnemyEmitter::Emit(std::vector<std::unique_ptr<Enemy>>& enemies, const Math::Vector3& bossPos) {

	std::unique_ptr<Enemy> enemy;
	enemy = std::make_unique<Enemy>();
	enemy->Initialize();
	enemy->SetPlayer(SceneGame::GetInstance()->GetPlayer()->GetEntity());
	enemy->GetEntity()->RefTransform().translate.x = bossPos.x + Random::Create(-10.0f, 15.0f);
	enemy->GetEntity()->RefTransform().translate.y = bossPos.y + Random::Create(-10.0f, 10.0f);
	enemies.push_back(std::move(enemy));

}

} // namespace Atrum