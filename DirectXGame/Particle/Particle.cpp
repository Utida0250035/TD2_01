#include "Particle.h"


namespace Atrum {

Particles::Particles() {

	isFinish_ = false;
	srand(static_cast<unsigned int>(time(nullptr)));

}

} // namespace Atrum