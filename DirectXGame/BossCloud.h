#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "System//CmpHitSphere.h"
#include "System//CmpMesh.h"
#include "System/CmpHealthHit.h"
#include "System/CmpLhRigidBody.h"
#include "System/Entity.h"

#include "BossAttack.h"
#include "BossLightning.h"

namespace Atrum {

class BossCloud {

private:

	// 攻撃リスト
	std::unique_ptr<BossAttack> attack_;

	// エンティティ
	std::unique_ptr<Entity> entity_;

public:

	// 初期化
	void Initialize(const Math::Vector3 &pos);

	// 更新
	void Update();

	// 描画
	void Draw();

	~BossCloud() = default;
	BossCloud() = default;
};

} // namespace Atrum