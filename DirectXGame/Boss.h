#pragma once


#include "KamataEngine.h"

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "System//CmpHitSphere.h"
#include "System//CmpMesh.h"
#include "System/CmpLhRigidBody.h"
#include "System/CmpHealthHit.h"
#include "System/Entity.h"

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "../Hash/Hash64.h"
#include "../Math/Transform.h"
#include "../System/DataComponent.h"
#include "../System/DrawComponent.h"
#include "../System/EntityType.h"
#include "../System/UpdComponent.h"

// ボス
class Boss final {
private:
	// エンティティ
	std::unique_ptr<Atrum::Entity> entity_ = nullptr;

public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	Atrum::Entity* GetEntity() const { return entity_.get(); }
};

namespace Atrum {

class CmpBoss : public UpdComponent {

private:
	::Boss* anotherOwner_ = nullptr;


public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::MOVEMENT; }

	void ResolveDependence() override;
	void Initialize() override;
	void Update() override;

	void SetAnotherOwner(::Boss* owner) { anotherOwner_ = owner; }
};

} // namespace Atrum
