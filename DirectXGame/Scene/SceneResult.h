#pragma once

#include "Scene.h"
#include <cstdint>

namespace Atrum {

class Entity;

class SceneResult final : public Scene {

private:

	int32_t choice_ = 0;

	static inline constexpr int32_t kChoiceMax = 2;

public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneResult* GetInstance() {

		static SceneResult instance;

		return &instance;
	}
};

} // namespace Atrum