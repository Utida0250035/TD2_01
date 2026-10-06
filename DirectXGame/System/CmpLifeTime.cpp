#include "CmpLifeTime.h"
#include "../Time/DeltaTime.h"
#include "Entity.h"

namespace Atrum {

	void CmpLifeTime::ResolveDependence() {

		frameDeltaTime_ = FrameDeltaTime::GetInstance();

	}

	void CmpLifeTime::Initialize() {

		lifeTimer_ = lifeTime_;

	}

	void CmpLifeTime::Update() {

		if (lifeTimer_ > 0.0f) {

		    lifeTimer_ -= frameDeltaTime_;

			if (lifeTimer_ <= 0.0f) {
		    
				lifeTimer_ = 0.0f;

				RefOwner().SetState(Entity::State::Destroy);
			
			}

	    }

	}

}