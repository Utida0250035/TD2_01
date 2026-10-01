#pragma once

#include "../System/DataComponent.h"
#include "Entity.h"
#include <cassert>
#include <memory>

namespace Atrum {

class CmpParry : public DataComponent {
private:
	std::unique_ptr<Entity> entParry_ = nullptr;

public:
	void SetEntParry(Entity* entParry) { entParry_.reset(entParry); }
	Entity& RefEntParry() {
		assert(entParry_);
		return *entParry_;
	}
};

} // namespace Atrum