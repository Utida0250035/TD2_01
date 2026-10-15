#pragma once

#include "./Math/Vector3.h"
#include "System/Entity.h"

namespace Atrum {

class BossAttack {

protected:

	// エンティティ
	std::unique_ptr<Entity> entity_ = nullptr;

public:
	// 初期化
	virtual void Initialize(const Math::Vector3& emitPos) = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了
	virtual void Finalize() {};

	// 有効状態
	bool IsActive() { return entity_->GetState() == Entity::State::Active; };

};

} // namespace Atrum
