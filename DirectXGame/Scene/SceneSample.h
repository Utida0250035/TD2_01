#pragma once

#include "Scene.h"

namespace Atrum {

class Entity;

class SceneSample final : public Scene {
public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneSample* GetInstance() {

		static SceneSample instance;

		return &instance;
	}
};

} // namespace Atrum