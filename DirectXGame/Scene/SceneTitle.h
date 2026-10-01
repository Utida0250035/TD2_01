#pragma once

#include "Scene.h"

namespace Atrum {

class Entity;

class SceneTitle final : public Scene {
public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneTitle* GetInstance() {

		static SceneTitle instance;

		return &instance;
	}
};

} // namespace Atrum