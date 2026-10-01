#include "../System/EntityFactory.h"
#include "../Math/AeVector3.h"
#include "../Math/Transform.h"
#include "../System/CmpChildEntitys.h"
#include "../System/CmpDirection.h"
#include "../System/CmpDrawBullet.h"
#include "../System/CmpDrawHitSphere.h"
#include "../System/CmpEnemy.h"
#include "../System/CmpHealthHit.h"
#include "../System/CmpHitSphere.h"
#include "../System/CmpLhRigidBody.h"
#include "../System/CmpMesh.h"
#include "../System/CmpMeshSphere.h"
#include "../System/CmpOrbiter.h"
#include "../System/CmpParry.h"
#include "../System/CmpReflectableBullet.h"
#include "../System/CmpScroll.h"
#include "../System/CmpSwing.h"
#include "../System/Entity.h"
#include "../System/Playable/CmpMoveByInput.h"
#include <numbers>
#include <stdlib.h>
#include <time.h>

namespace Atrum {

std::unique_ptr<Entity> EntityFactory::PlayerMononofu() {

	std::unique_ptr<Entity> playerMononofu = std::make_unique<Entity>();
	playerMononofu->SetEntityType(EntityType::PLAYER);

	Math::TransformLH initialTransform = Math::TransformLH{};

	initialTransform.quaternion = Math::Quaternion::FromAxisAngle(Math::Vector3::Up(), -std::numbers::pi_v<float> * 0.5f);
	initialTransform.translate = {-8.0f, -3.0f, 0.0f};

	playerMononofu->SetInitialTransform(initialTransform);

	playerMononofu->AddDataCmp<CmpDirection>();
	playerMononofu->AddUpdCmp<CmpLhRigidBody>();
	playerMononofu->AddUpdCmp<Player::CmpMoveByInput2D>();
	playerMononofu->AddUpdCmp<CmpHitSphere>();
	playerMononofu->AddUpdCmp<CmpHealthHit>();
	playerMononofu->AddDrawCmp<CmpMesh>();
	playerMononofu->AddDrawCmp<CmpDrawHitSphere>();
	playerMononofu->AddDataCmp<CmpParry>();
	playerMononofu->AddUpdCmp<CmpChildEntitys>();
	playerMononofu->AddUpdCmp<CmpSwing>();

	CmpMesh* cmpMesh = playerMononofu->GetDrawCmp<CmpMesh>();

	cmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("playerHako"));

	CmpLhRigidBody* cmpLhRigidBody = playerMononofu->GetUpdCmp<CmpLhRigidBody>();

	cmpLhRigidBody->SetGravity({0.0f, -45.0f, 0.0f});

	CmpHitSphere* cmpHitSphere = playerMononofu->GetUpdCmp<CmpHitSphere>();

	cmpHitSphere->SetSpheres({
	    {{}, 0.5f},
	});

	CmpHealthHit* cmpHealthHit = playerMononofu->GetUpdCmp<CmpHealthHit>();

	cmpHealthHit->SetHurterTypes({EntityType::ENEMY, EntityType::ENEMY_WEAPON});

	CmpParry* cmpParry = playerMononofu->GetDataCmp<CmpParry>();
	cmpParry->SetEntParry(Parry(playerMononofu.get()).release());

	CmpChildEntitys* cmpChildEntitys = playerMononofu->GetUpdCmp<CmpChildEntitys>();

	Entity* weaponPivot = new Entity();
	weaponPivot->AddUpdCmp<CmpChildEntitys>();
	CmpSwing* cmpSwing = playerMononofu->GetUpdCmp<CmpSwing>();
	cmpSwing->SetPivot(weaponPivot);
	cmpSwing->SetSwingSpeed(std::numbers::pi_v<float> * 8.0f);
	cmpSwing->SetSwingDuration(0.25f);

	Math::TransformLH weaponPivotInitialTransform{};

	weaponPivotInitialTransform.quaternion = Math::Quaternion::FromAxisAngle(Math::Vector3::Forward(), -std::numbers::pi_v<float>);

	weaponPivot->SetInitialTransform(weaponPivotInitialTransform);

	weaponPivot->SetIsInheritParentPositionOnly(false);

	cmpChildEntitys->AddChilds({weaponPivot});

	CmpChildEntitys* weaponPivotCmpChildEntitys = weaponPivot->GetUpdCmp<CmpChildEntitys>();

	Entity* weapon = new Entity();
	weapon->AddDrawCmp<CmpMesh>();

	Math::TransformLH weaponInitialTransform{};

	weaponInitialTransform.translate = {-1.0f, 0.0f, 0.0f};
	weaponInitialTransform.quaternion = Math::Quaternion::FromAxisAngle(Math::Vector3::Forward(), std::numbers::pi_v<float>);

	weapon->SetInitialTransform(weaponInitialTransform);

	CmpMesh* weaponCmpMesh = weapon->GetDrawCmp<CmpMesh>();

	weaponCmpMesh->SetModel(KamataEngine::Model::CreateFromOBJ("katana"));

	weaponPivotCmpChildEntitys->AddChilds({weapon});

	playerMononofu->ResolveDependences();
	playerMononofu->Initialize();

	return std::move(playerMononofu);
}

std::unique_ptr<Entity> EntityFactory::EnemyAstro(Entity* player) {

	std::unique_ptr<Entity> enemyAstro = std::make_unique<Entity>();
	enemyAstro->SetEntityType(EntityType::ENEMY);

	Math::TransformLH initialTransform = Math::TransformLH{};

	initialTransform.quaternion = Math::Quaternion::FromAxisAngle(Math::Vector3::Up(), std::numbers::pi_v<float> * 0.5f);
	initialTransform.translate = {8.0f, 0.0f, 0.0f};

	enemyAstro->SetInitialTransform(initialTransform);

	enemyAstro->AddUpdCmp<CmpHitSphere>();
	enemyAstro->AddUpdCmp<CmpHealthHit>();
	enemyAstro->AddUpdCmp<CmpEnemy>();
	enemyAstro->AddUpdCmp<CmpReflectableBullet>();
	enemyAstro->AddDrawCmp<CmpMeshSphere>();
	enemyAstro->AddDrawCmp<CmpDrawHitSphere>();
	enemyAstro->AddDrawCmp<CmpDrawBullet>();
	enemyAstro->AddUpdCmp<CmpChildEntitys>();

	CmpMeshSphere* cmpMeshSphere = enemyAstro->GetDrawCmp<CmpMeshSphere>();
	cmpMeshSphere->SetColor({0.75f, 0.25f, 0.125f, 1.0f});
	cmpMeshSphere->SetRadius(1.0f);

	CmpHitSphere* cmpHitSphere = enemyAstro->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 0.8f}
    });

	CmpHealthHit* cmpHealthHit = enemyAstro->GetUpdCmp<CmpHealthHit>();
	cmpHealthHit->SetHurterTypes({EntityType::PLAYER_ITEM});
	cmpHealthHit->SetInitialHealthCount(32);

	CmpChildEntitys* cmpChildEntitys = enemyAstro->GetUpdCmp<CmpChildEntitys>();

	const float orbiterHitRadius = 0.4f;
	const float orbiterRadius = 0.5f;
	const float distance = 1.5f;
	const Math::Vector4 orbiterColor = {0.75f, 0.25f, 0.125f, 1.0f};

	CmpEnemy* cmpEnemy = enemyAstro->GetUpdCmp<CmpEnemy>();
	cmpEnemy->SetPlayer(player);

	std::vector<Entity*> orbiters{};

	const Math::Vector3 defaultInitialTransform = {distance, 0.0f, 0.0f};
	const Math::Vector3 defaultOrbitAxis = Math::Vector3::Up();

	struct OrbitConfig {
		Math::Vector3 axisRotateByDegree{};
		float rotateSpeedByDegree;
	};

	std::vector<OrbitConfig> orbitConfigs{
	    {{0.0f, 0.0f, 0.0f},    180.0f},
        {{45.0f, 0.0f, 0.0f},   150.0f},
        {{30.0f, 0.0f, 15.0f},  200.0f},
        {{90.0f, 0.0f, 30.0f},  360.0f},
        {{60.0f, 30.0f, 70.0f}, 200.0f}
    };

	Math::TransformLH orbiterInitialTransform{};

	Math::Matrix4x4 orbitAxisRotateMatrix{};

	CmpMeshSphere* orbiterCmpMeshSphere = nullptr;
	CmpHitSphere* orbiterCmpHitSphere = nullptr;
	CmpOrbiter* cmpOrbiter = nullptr;

	for (size_t i = 0; i < orbitConfigs.size(); ++i) {
		orbiters.emplace_back(new Entity());
		orbiters.back()->SetEntityType(EntityType::ENEMY_WEAPON);
		orbiters.back()->AddUpdCmp<CmpOrbiter>();
		orbiters.back()->AddDrawCmp<CmpMeshSphere>();
		orbiters.back()->AddUpdCmp<CmpHitSphere>();

		orbiterCmpMeshSphere = orbiters.back()->GetDrawCmp<CmpMeshSphere>();
		orbiterCmpMeshSphere->SetColor(orbiterColor);
		orbiterCmpMeshSphere->SetRadius(orbiterRadius);

		orbiterCmpHitSphere = orbiters.back()->GetUpdCmp<CmpHitSphere>();
		orbiterCmpHitSphere->SetSpheres({
		    {{}, orbiterHitRadius}
        });

		orbitAxisRotateMatrix = Math::Matrix4x4::Rotate(orbitConfigs[i].axisRotateByDegree / 180.0f * std::numbers::pi_v<float>);

		orbiterInitialTransform.translate = orbitAxisRotateMatrix.Transform(defaultInitialTransform);
		orbiters[i]->SetInitialTransform(orbiterInitialTransform);

		cmpOrbiter = orbiters[i]->GetUpdCmp<CmpOrbiter>();
		cmpOrbiter->SetRotateAxis(orbitAxisRotateMatrix.Transform(defaultOrbitAxis));
		cmpOrbiter->SetRotateSpeed(orbitConfigs[i].rotateSpeedByDegree / 180.0f * std::numbers::pi_v<float>);
	}

	cmpChildEntitys->AddChilds(orbiters);

	enemyAstro->ResolveDependences();

	return std::move(enemyAstro);
}

std::vector<std::unique_ptr<Entity>> EntityFactory::ScrollSpheres() {

	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<std::unique_ptr<Entity>> scrollSpheres{};

	float scrollSpeed = 0.0f;

	CmpScroll* cmpScroll = nullptr;
	CmpMeshSphere* cmpMeshSphere = nullptr;

	int sign = 0;

	Math::TransformLH initialTransform{};

	for (size_t i = 0; i < 32; ++i) {

		std::unique_ptr<Entity> scrollSphere = std::make_unique<Entity>();

		initialTransform.translate = {static_cast<float>(rand() % 33 - 32), static_cast<float>(rand() % 17 - 8), static_cast<float>(rand() % 11 + 20)};

		scrollSphere->SetInitialTransform(initialTransform);

		scrollSphere->AddUpdCmp<CmpScroll>();
		scrollSphere->AddDrawCmp<CmpMeshSphere>();

		cmpScroll = scrollSphere->GetUpdCmp<CmpScroll>();

		scrollSpeed = static_cast<float>(rand() % 5 + 4);

		sign = rand() % 2;

		if (sign == 0) {
			scrollSpeed *= -1.0f;
		}

		cmpScroll->SetScrollSpeed(scrollSpeed);

		cmpMeshSphere = scrollSphere->GetDrawCmp<CmpMeshSphere>();

		cmpMeshSphere->SetColor({0.25f, 0.1f, 0.25f, 1.0f});

		cmpMeshSphere->SetRadius(static_cast<float>(rand() % 16 + 16) * 0.03125f);

		scrollSphere->ResolveDependences();

		scrollSpheres.emplace_back(std::move(scrollSphere));
	}

	return scrollSpheres;
}

std::unique_ptr<Entity> EntityFactory::LinearEnemyBullet(const Math::Vector3& velocity, const Math::Vector3& position) {

	std::unique_ptr<Entity> bullet = std::make_unique<Entity>();

	Math::TransformLH initialTransform{};

	initialTransform.translate = position;

	bullet->SetInitialTransform(initialTransform);

	bullet->SetEntityType(EntityType::ENEMY_WEAPON);

	bullet->AddUpdCmp<CmpLhRigidBody>();
	bullet->AddUpdCmp<CmpHitSphere>();
	bullet->AddDrawCmp<CmpMeshSphere>();

	CmpHitSphere* cmpHitSphere = bullet->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 0.5f}
    });

	CmpMeshSphere* cmpMeshSphere = bullet->GetDrawCmp<CmpMeshSphere>();
	cmpMeshSphere->SetRadius(0.5f);
	cmpMeshSphere->SetColor({0.75f, 0.1f, 0.1f, 1.0f});

	bullet->ResolveDependences();
	bullet->Initialize();

	CmpLhRigidBody* cmpLhRigidBody = bullet->GetUpdCmp<CmpLhRigidBody>();
	cmpLhRigidBody->RefVelocity() = velocity;
	cmpLhRigidBody->SetIsUseSpaceEnd(false);

	return std::move(bullet);
}

std::unique_ptr<Entity> EntityFactory::Parry(Entity* parent) {

	std::unique_ptr<Entity> parry = std::make_unique<Entity>();
	parry->SetEntityType(EntityType::PLAYER_WEAPON);

	Math::TransformLH initialTransform{};

	initialTransform.translate = {0.25f, 1.0f, 0.0f};

	parry->SetInitialTransform(initialTransform);

	parry->AddUpdCmp<CmpHitSphere>();

	CmpHitSphere* cmpHitSphere = parry->GetUpdCmp<CmpHitSphere>();
	cmpHitSphere->SetSpheres({
	    {{}, 2.0f}
    });

	parry->SetParent(parent);

	parry->ResolveDependences();

	return std::move(parry);
}

} // namespace Atrum