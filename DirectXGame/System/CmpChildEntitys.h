#pragma once
#include "../System/Entity.h"
#include "../System/UpdComponent.h"
#include <memory>
#include <vector>
#include <iostream>

namespace Atrum {

class CmpChildEntitys : public UpdComponent {
private:
	std::vector<std::unique_ptr<Entity>> childs_{};

public:

	constexpr UpdCmpGroup UpdGroup() const override { return UpdCmpGroup::NONE_GROUPING; }

	void Update() override {

		size_t index = 0;

		for (auto& child : childs_) {

			if (!child)
				continue;

			if (!child->GetParent()) {
				childs_.erase(childs_.begin() + index);
				continue;
			}

			Math::Vector3 pos = child->GetWorldPosition();

			index++;
		}
	}

	std::vector<std::unique_ptr<Entity>>& RefChilds() { return childs_; }
	void AddChilds(const std::vector<Entity*> childs) {
		for (auto* child : childs) {
			child->SetParent(&RefOwner());
			childs_.emplace_back();
			childs_.back().reset(child);
		}
	}
};

} // namespace Atrum