#include "../Time/DeltaTime.h"
#include "ParticleSample.h"
#include "../Camera/Camera.h"
#include "../Math/Matrix3x3.h"

namespace Atrum {

/// <summary>
///
/// </summary>
ParticleSample::ParticleSample() {}

void ParticleSample::Initialize() {}

void ParticleSample::Update() {

	for (auto& info : particleInfo_) {

		info.velocity +=  info.acceleration * FrameDeltaTime::GetInstance()->GetDeltaTime();
		info.position += info.velocity * FrameDeltaTime::GetInstance()->GetDeltaTime();

	}
}

void ParticleSample::Draw() {}

} // namespace Atrum