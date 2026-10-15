#pragma once

#include "../Math/Vector3.h"
#include "../System/UpdComponent.h"
#include <vector>

class Player;

namespace Atrum {

struct HitSphere {
	Math::Vector3 localOffset{};
	float radius;
};

class Entity;
class HitJudge;

class CmpHitSphere : public UpdComponent {
private:
	HitJudge* hitJudge_ = nullptr;

	// プレイヤー
	::Player* player_ = nullptr;

	std::vector<HitSphere> hitSpheres_{};
	std::vector<Entity*> hitEntitys_{};
	bool isActive_ = true;

	bool isEnemy_ = false;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::HIT_JUDGE; }

	void ResolveDependence() override;
	void Initialize() override;
	void Update() override;
	void Finalize() override;

	std::vector<HitSphere> GetSpheres() const { return hitSpheres_; }
	void SetSpheres(const std::vector<HitSphere>& hitSpheres) { hitSpheres_ = hitSpheres; }

	void SetIsActive(const bool isActive) { isActive_ = isActive; }
	bool GetIsActive() const { return isActive_; }

	void SetPlayer(::Player* player) { player_ = player; }
	::Player* GetPlayer() { return player_; }

	void SetEnemy() { isEnemy_ = true; }
	bool IsEnemy() const { return isEnemy_; }

	std::vector<Entity*> GetHitEntitys();
};

} // namespace Atrum