#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "../Hash/Hash64.h"
#include "../Math/Transform.h"
#include "../System/DataComponent.h"
#include "../System/DrawComponent.h"
#include "../System/EntityType.h"
#include "../System/UpdComponent.h"

namespace Atrum {

// コンテナとなるクラス
class Entity {
public:
	enum class State { Initialize, Sleep, Active, Destroy };

private:
	Entity* parent_ = nullptr;

	std::vector<Entity*> childs_{};

	uint64_t nameHash_ = 0;

	uint64_t id_ = 0;

	EntityType type_ = EntityType::NONE;

	struct UpdCmpBox {
		uint64_t priority = 0;

		UpdComponent* component = nullptr;

		UpdCmpBox(uint64_t p, UpdComponent* c) : priority(p), component(c) {}

		UpdComponent* operator->() { return component; }
		operator bool() { return component; }
	};

	struct DrawCmpBox {
		uint64_t priority = 0;

		DrawComponent* component = nullptr;

		DrawCmpBox(uint64_t p, DrawComponent* c) : priority(p), component(c) {}

		DrawComponent* operator->() { return component; }
		operator bool() { return component; }
	};

	Math::TransformLH transform_{};
	Math::TransformLH initialTransform_{};

	bool isInheritParentPositionOnly_ = false;

	std::vector<std::unique_ptr<UpdComponent>> updComponents_{};
	std::vector<std::unique_ptr<DrawComponent>> drawComponents_{};

	std::vector<UpdCmpBox> updCmpBoxes_{};
	std::vector<DrawCmpBox> drawCmpBoxes_{};

	std::vector<std::unique_ptr<DataComponent>> dataComponents_{};

	State state_ = State::Initialize;

public:
	Entity() = default;
	~Entity();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	template<typename T> UpdCmpBox& RefUpdCmpBox() {
		for (const auto& cmpBox : updCmpBoxes_) {
			if (auto p = dynamic_cast<T*>(cmpBox.component)) {
				return cmpBox;
			}
		}

		assert(false && "UpdCmpBox not found, can't get");

		return nullptr;
	}

	template<typename T> T* GetUpdCmp() {
		for (const auto& component : updComponents_) {
			if (auto p = dynamic_cast<T*>(component.get())) {
				return p;
			}
		}

		assert(false && "UpdCmp not found, can't get");

		return nullptr;
	}

	template<typename T> bool HasUpdCmp() const {
		for (const auto& component : updComponents_) {
			if (auto p = dynamic_cast<T*>(component.get())) {
				return true;
			}
		}

		return false;
	}

	template<typename T> void AddUpdCmp() {
		if (HasUpdCmp<T>()) {
			assert(false && "UpdComponent is already added to entity");

			return;
		}

		std::unique_ptr<UpdComponent> component = std::make_unique<T>();

		component->SetOwner(this);

		updCmpBoxes_.emplace_back(static_cast<uint64_t>(component->UpdGroup()), component.get());

		updComponents_.emplace_back(std::move(component));

		// 追加するたびに優先度でソート（昇順）
		std::sort(updCmpBoxes_.begin(), updCmpBoxes_.end(), [](const UpdCmpBox& a, const UpdCmpBox& b) { return a.priority < b.priority; });
	}

	template<typename T> DrawCmpBox& RefDrawCmpBox() {
		for (const auto& cmpBox : drawCmpBoxes_) {
			if (auto p = dynamic_cast<T*>(cmpBox.component)) {
				return cmpBox;
			}
		}

		assert(false && "DrawCmpBox not found, can't get");

		return nullptr;
	}

	template<typename T> T* GetDrawCmp() {
		for (const auto& component : drawComponents_) {
			if (auto p = dynamic_cast<T*>(component.get())) {
				return p;
			}
		}

		assert(false && "DradCmp not found, can't get");

		return nullptr;
	}

	template<typename T> bool HasDrawCmp() const {
		for (const auto& component : drawComponents_) {
			if (auto p = dynamic_cast<T*>(component.get())) {
				return true;
			}
		}

		return false;
	}

	template<typename T> void AddDrawCmp() {
		if (HasDrawCmp<T>()) {
			assert(false && "DrawComponent is already added to entity");

			return;
		}

		std::unique_ptr<DrawComponent> component = std::make_unique<T>();

		component->SetOwner(this);

		drawCmpBoxes_.emplace_back(static_cast<uint64_t>(component->DrawGroup()), component.get());

		drawComponents_.emplace_back(std::move(component));

		// 追加するたびに優先度でソート（昇順）
		std::sort(drawCmpBoxes_.begin(), drawCmpBoxes_.end(), [](const DrawCmpBox& a, const DrawCmpBox& b) { return a.priority < b.priority; });
	}

	template<typename T> T* GetDataCmp() {
		for (const auto& component : dataComponents_) {
			if (auto p = dynamic_cast<T*>(component.get())) {
				return p;
			}
		}

		assert(false && "DataCmp not found, can't get");

		return nullptr;
	}

	template<typename T> bool HasDataCmp() const {
		for (const auto& component : dataComponents_) {
			if (auto p = dynamic_cast<T*>(component.get())) {
				return true;
			}
		}

		return false;
	}

	template<typename T> void AddDataCmp() {
		if (HasDataCmp<T>()) {
			assert(false && "DataComponent is already added to entity");

			return;
		}

		std::unique_ptr<DataComponent> component = std::make_unique<T>();

		component->SetOwner(this);

		dataComponents_.emplace_back(std::move(component));
	}

	void ResolveDependences();

	explicit Entity(const std::string& name) : nameHash_(Hash64(name)) {}

	Math::TransformLH& RefTransform() { return transform_; }
	const Math::TransformLH& GetTransform() { return transform_; }

	Math::TransformLH GetInitialTransform() { return initialTransform_; }
	void SetInitialTransform(const Atrum::Math::TransformLH& initialTransform) { initialTransform_ = initialTransform; }

	uint64_t GetHash() const { return nameHash_; }

	uint64_t GetId() const { return id_; }

	State GetState() const { return state_; }

	void SetState(const State& state) { state_ = state; }

	Math::Matrix4x4 GetWorldMatrix() const;
	Math::Vector3 GetWorldPosition() const;

	bool IsActive() const { return state_ == State::Active; }

	void RemoveChild(Entity* target) { std::erase(childs_, target); }

	void SetParent(Entity* newParent);
	const Entity* GetParent() { return parent_; };

	void SetHash(const uint64_t nameHash) { nameHash_ = nameHash; }

	void SetHash(const std::string& name) { nameHash_ = Hash64(name); }

	void SetId(const uint64_t id) { id_ = id; }

	void RequestDestroy() { state_ = State::Destroy; }

	void SetEntityType(const EntityType& type) { type_ = type; }
	EntityType GetEntityType() const { return type_; }

	void SetIsInheritParentPositionOnly(const bool isInheritPositionOnly) { isInheritParentPositionOnly_ = isInheritPositionOnly; }
};

} // namespace Atrum