#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "System//CmpHitSphere.h"
#include "System//CmpMesh.h"
#include "System/CmpHealthHit.h"
#include "System/CmpLhRigidBody.h"
#include "System/Entity.h"

#include "Math/lerp.h"
#include "BossAttack.h"
#include "BossLightning.h"

namespace Atrum {

class BossCloud {

private:

	// 攻撃リスト
	std::unique_ptr<BossAttack> attack_;

	// エンティティ
	std::unique_ptr<Entity> entity_;

	// 有効状態
	bool isActive_ = true;

	// メッシュコンポーネント
	CmpMesh* cmpMesh_ = nullptr;

	float alpha_ = 0.0f;

	// 経過時間
	float timer_ = 0.0f;

public:

	// 初期化
	void Initialize(const Math::Vector3 &pos);

	// 更新
	void Update();

	// 描画
	void Draw();

	bool IsActive() const { return isActive_; }

	~BossCloud() = default;
	BossCloud() = default;
};

} // namespace Atrum