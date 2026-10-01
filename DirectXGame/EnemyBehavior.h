#pragma once

class Enemy;

class EnemyBehavior {

protected:
	Enemy* pEnemy_ = nullptr;

public:
	virtual ~EnemyBehavior() = default;

	virtual void Enter() {}

	virtual void Execute() = 0;

	virtual void Exit() {}

	void SetOwner(Enemy* pEnemy) { pEnemy_ = pEnemy;}

};