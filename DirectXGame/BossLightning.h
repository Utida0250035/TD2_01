#pragma once

#include "Math/Vector3.h"
#include "System/CmpHitSphere.h"
#include "System/Entity.h"
#include <memory>

namespace Atrum {

class FrameDeltaTime;

class BossLightning {
private:

	CmpHitSphere* cmpHitSphere_ = nullptr;
	std::unique_ptr<Entity> entity_ = nullptr;

	float growTimer_ = 0.0f;
	float growTime_ = 0.01562f;

	size_t sphereMax_ = 16;
	size_t sphereCount_ = 0;

	std::vector<Math::Vector3> offsets_{
		{1.0f, -1.5f, 0.0f}, {0.0f, -1.5f, 0.0f}, {-0.6f, -1.5f, 0.0f}, {-0.4f, -1.5f, 0.0f}
	};

	inline static constexpr float kSphereRadius = 1.0f;

	FrameDeltaTime* frameDeltaTime_ = nullptr;

public:
	void Initialize(const Math::Vector3& emitPos);
	void Update();
	void Draw();
	void Finalize();

	bool IsActive();
};

} // namespace Atrum