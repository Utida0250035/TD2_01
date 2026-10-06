#pragma once

#include "Scene.h"

namespace Atrum {

class Entity;

class SceneGame final : public Scene {
public:
	void EnterScene() override;
	void Update() override;
	void Draw() override;
	void ExitScene() override;

	static SceneGame* GetInstance() {

		static SceneGame instance;

		return &instance;
	}
};

} // namespace Atrum