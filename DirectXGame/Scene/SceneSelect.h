#pragma once

#include "Scene.h"

namespace Atrum {

class Entity;

class SceneSelect final : public Scene {
public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneSelect* GetInstance() {

		static SceneSelect instance;

		return &instance;
	}
};

} // namespace Atrum