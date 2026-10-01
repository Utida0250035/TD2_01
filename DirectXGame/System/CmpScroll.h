#pragma once

#include "../Math/AeVector3.h"
#include "../System/Entity.h"
#include "../System/UpdComponent.h"
#include "../Time/DeltaTime.h"

namespace Atrum {

class CmpScroll : public UpdComponent {

private:
	FrameDeltaTime* frameDeltaTime_ = nullptr;

	float scrollSpeed_{};

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::ANIMATION; }

	void ResolveDependence() override { frameDeltaTime_ = FrameDeltaTime::GetInstance(); }

	void Update() override { 
		Math::Vector3& translate = RefOwner().RefTransform().translate;
		translate.x += scrollSpeed_ * frameDeltaTime_->GetDeltaTime(); 

		constexpr float sideEndX = 48.0f;

		if (translate.x <= -sideEndX) {
		
			translate.x = sideEndX;
		
		} else if (translate.x >= sideEndX) {
		
			translate.x = -sideEndX;
		
		}

	}

	void SetScrollSpeed(const float speed) { scrollSpeed_ = speed; }
};

} // namespace Atrum