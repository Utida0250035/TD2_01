#pragma once

#include "Math/Vector3.h"
#include "System/CmpHitSphere.h"
#include "System/Entity.h"
#include "BossAttack.h"

#include <memory>

namespace Atrum {

class FrameDeltaTime;

class BossLightning : public BossAttack {
private:

	CmpHitSphere* cmpHitSphere_ = nullptr;
	

	float growTimer_ = 0.0f;
	float growTime_ = 0.01562f;

	size_t sphereMax_ = 16;
	size_t sphereCount_ = 0;

	std::vector<Math::Vector3> offsets_{
	    {1.0f, -2.5f, 0.0f},
        {-0.7f, -3.0f, 0.0f},
        {-0.3f, -4.0f, 0.0f}
	};

	inline static constexpr float kSphereRadius = 1.0f;

	FrameDeltaTime* frameDeltaTime_ = nullptr;

public:
	void Initialize(const Math::Vector3& emitPos) override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

};

} // namespace Atrum