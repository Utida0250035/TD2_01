#pragma once

#include "Particle.h"

namespace Atrum {

class ParticleSample : public Particles {

private:
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	///
	/// </summary>
	ParticleSample();

	~ParticleSample() = default;
};

} // namespace Atrum