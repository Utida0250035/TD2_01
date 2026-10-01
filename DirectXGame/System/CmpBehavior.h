#pragma once

#include "../System/BehaviorBox.h"
#include "../System/DataComponent.h"
#include "../System/UpdComponent.h"
#include <cassert>

namespace Atrum {

class CmpBehavior : UpdComponent {
private:
	BehaviorBox behaviorBox_{};
	std::unique_ptr<Behavior> (*initialBehaviorfactory_)(void) = nullptr;

public:
	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::MOVEMENT; }

	void Initialize() override {
		assert(initialBehaviorfactory_);
		behaviorBox_.Transition(initialBehaviorfactory_().release());
	};

	void Update() override { behaviorBox_.Execute(); };

	template<typename T> void SetInitialBehavior() {
		initialBehaviorfactory_ = []() -> std::unique_ptr<Behavior> { return std::make_unique<T>; };
	};
};

} // namespace Atrum