#pragma once

#include "Enemy.h"

class ShieldEnemy : public Enemy {

private:
	void MoveMotion(const float deltaTime) override;

	bool isStan_ = false;

public:
	void SetIsStan(const bool isParrying) { isStan_ = isParrying; }
};