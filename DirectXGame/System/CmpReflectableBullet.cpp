#include "../System/CmpReflectableBullet.h"
#include "../Audio/Audio.h"
#include "../ConstantVal.h"
#include "../System/CmpHitSphere.h"
#include "../System/CmpLhRigidBody.h"
#include "../System/CmpMeshSphere.h"
#include "../System/Entity.h"

#include <iostream>

namespace Atrum {

void CmpReflectableBullet::Initialize() {

	for (auto& bullet : reflectableBullets_) {
		if (bullet)
			bullet->Finalize();
	}

	reflectableBullets_.clear();
}

void CmpReflectableBullet::Update() {

	Math::Vector3 position{};

	size_t index = 0;

	CmpHitSphere* cmpHitSphere = nullptr;

	CmpLhRigidBody* cmpLhRigidBody = nullptr;

	CmpMeshSphere* cmpMeshSphere = nullptr;

	for (auto& bullet : reflectableBullets_) {

		if (bullet) {

			if (bullet->GetState() != Entity::State::Active) {

				reflectableBullets_[index]->Finalize();

				reflectableBullets_.erase(reflectableBullets_.begin() + index);

				continue;
			}

			bullet->Update();

			cmpHitSphere = bullet->GetUpdCmp<CmpHitSphere>();

			cmpLhRigidBody = bullet->GetUpdCmp<CmpLhRigidBody>();

			cmpMeshSphere = bullet->GetDrawCmp<CmpMeshSphere>();

			std::vector<Entity*> hitEntitys = cmpHitSphere->GetHitEntitys();

			if (bullet->GetEntityType() == EntityType::ENEMY_WEAPON) {

				for (Entity* hitEntity : hitEntitys) {

					if (hitEntity->GetEntityType() == reflecterType_) {

						bullet->SetEntityType(EntityType::PLAYER_ITEM);
						cmpLhRigidBody->RefVelocity() = {};
						cmpMeshSphere->SetColor({0.1f, 0.1f, 0.7f, 1.0f});

						bullet->RefTransform().translate = bullet->GetWorldPosition();

						bullet->SetParent(nullptr);

						Atrum::Audio::Manager::GetInstance()->Play(Atrum::Audio::Manager::GetInstance()->Get("./Resources/seParry.mp3"));

						break;
					}
				}
			} else if (bullet->GetEntityType() == EntityType::PLAYER_ITEM) {

				constexpr float reflectedSpeed = 16.0f;

				cmpLhRigidBody->RefVelocity() = (RefOwner().GetWorldPosition() - bullet->GetWorldPosition()).Normalized() * reflectedSpeed;

				bool isHit = false;

				for (Entity* hitEntity : hitEntitys) {

					if (hitEntity->GetEntityType() == EntityType::ENEMY) {

						isHit = true;

						reflectableBullets_[index]->SetState(Entity::State::Sleep);

						break;
					}
				}

				if (isHit) {
					index++;
					continue;
				}
			}

			position = bullet->GetWorldPosition();

			if (position.x >= kRightEndX + kSpaceSideBuffer || position.x <= kLeftEndX - kSpaceSideBuffer || position.y >= kCeilHeight + kSpaceTopBuffer ||
			    position.y <= kGroundHeight - kSpaceBottomBuffer) {

				reflectableBullets_[index]->Finalize();

				reflectableBullets_.erase(reflectableBullets_.begin() + index);

				continue;
			}
		}

		index++;
	}
}

void CmpReflectableBullet::Shot(Entity* bullet) {
	reflectableBullets_.emplace_back();

	reflectableBullets_.back().reset(bullet);
}

} // namespace Atrum